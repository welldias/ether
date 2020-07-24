#include <glad/glad.h> 

#include <GLFW\glfw3.h>

#include <Ether.h>
#include <ether_glfw_driver.h>
#include <stdio.h>

#define VEC3_BACK     {0,  0, -1}
#define VEC3_DOWN     {0, -1,  0}
#define VEC3_FORWARD  {0,  0,  1}
#define VEC3_LEFT     {-1, 0,  0}
#define VEC3_RIGHT    {1,  0,  0}
#define VEC3_UP       {0,  1,  0}

struct _Ether_Cube {
	int totalVestices;
	int totalTriangles;

	float* vertices;
	unsigned int* triangles;
};

typedef struct _Ether_Cube EtherCube;

/* converting rgb color to opengl format */
//float out = (1.0f / 255) * byte_in;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void build_planet_mash(EtherCube* cube, int resolution);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;

EtherCamera camera;
float lastX = 800 / 2.0f;
float lastY = 600 / 2.0f;
EtherBool firstMouse = TRUE;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


int main()
{
	ether_camera_gl_configure(&camera, 0.0f, 0.0f, 15.0f, 0.0f, 1.0f, 0.0f, -90.0f, 0.0f);

	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Ether Opengl", NULL, NULL);
	if (window == NULL)
	{
		printf("Failed to create GLFW window\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD\n");
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	//glEnable(GL_DEPTH_TEST);


	// build and compile our shader program
	// ------------------------------------
	EtherShader shaderId = 0;
	if (ether_shader_load("src/bin/opengl10.vs", "src/bin/opengl10.fs", &shaderId) != 0)
	{
		printf("Failed to load and compile shaders\n");
		return -1;
	}

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	size_t countVertices = 0;
	size_t countIndices = 0;
	size_t totalIndices = 0;

	EtherCube cube;
	memset(&cube, 0, sizeof(EtherCube));

	build_planet_mash(&cube, 15);

	countVertices = sizeof(float) * cube.totalVestices * 3;
	countIndices = sizeof(int) * cube.totalTriangles;
	totalIndices = cube.totalTriangles;

	float* vertices = cube.vertices;
	unsigned int* indices = cube.triangles;

	vec3 spherePositions[] = {
	{  0.0f,  0.0f,   0.0f },
	{  2.0f,  5.0f, -15.0f },
	{ -1.5f, -2.2f,  -2.5f },
	{ -3.8f, -2.0f, -12.3f },
	{  2.4f, -0.4f,  -3.5f },
	{ -1.7f,  3.0f,  -7.5f },
	{  1.3f, -2.0f,  -2.5f },
	{  1.5f,  2.0f,  -2.5f },
	{  1.5f,  0.2f,  -1.5f },
	{ -1.3f,  1.0f,  -1.5f },
	};

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, countVertices, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, countIndices, indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// uncomment this call to draw in wireframe polygons.
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

		// draw our first triangle
		ether_shader_use(shaderId);

		// create transformations
		mat4 projection;
		glm_perspective(glm_rad(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f, projection);
		ether_shader_uniform_mat4(shaderId, "projection", projection);

		mat4 view;
		ether_camera_gl_view_matrix_get(&camera, view);
		ether_shader_uniform_mat4(shaderId, "view", view);

		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < 10; i++)
		{
			mat4 model = GLM_MAT4_IDENTITY_INIT;

			glm_translate(model, spherePositions[i]);

			float angle = 20.0f * i;
			vec3 v1; v1[0] = 1.0f; v1[1] = 0.3f; v1[2] = 0.5f;
			glm_rotate(model, glm_rad(angle), v1);
			ether_shader_uniform_mat4(shaderId, "model", model);

			glDrawElements(GL_TRIANGLES, (GLsizei)totalIndices, GL_UNSIGNED_INT, 0);
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	ether_shader_unload(shaderId);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float cameraSpeed = (float)2.5 * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		ether_camera_gl_process_keyboard(&camera, ETHER_CAMERA_MOVMENT_FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		ether_camera_gl_process_keyboard(&camera, ETHER_CAMERA_MOVMENT_BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		ether_camera_gl_process_keyboard(&camera, ETHER_CAMERA_MOVMENT_LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		ether_camera_gl_process_keyboard(&camera, ETHER_CAMERA_MOVMENT_RIGHT, deltaTime);
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = FALSE;
	}

	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos;  // reversed since y-coordinates go from bottom to top

	lastX = (float)xpos;
	lastY = (float)ypos;

	ether_camera_gl_process_mouse_movement(&camera, xoffset, yoffset, TRUE);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	ether_camera_gl_process_mouse_scroll(&camera, (float)yoffset);
}

void build_planet_mash(EtherCube* cube, int resolution)
{
	vec3 modelSide[] = {
		VEC3_UP,
		VEC3_FORWARD,
		VEC3_DOWN,
		VEC3_BACK,
		VEC3_LEFT,
		VEC3_RIGHT,
	};

	cube->totalVestices  = 0;
	cube->totalTriangles = 0;
	
	int z = 0;

	for (z = 0; z < 6; z++) {
		cube->totalVestices  += resolution * resolution;
		cube->totalTriangles += (resolution - 1) * (resolution - 1) * 6;
	}

	cube->vertices  = (float*)malloc(sizeof(float) * cube->totalVestices * 3);
	cube->triangles = (unsigned int*)malloc(sizeof(int) * cube->totalTriangles);

	int i = 0;
	int t = 0;

	vec3 axisA;
	vec3 axisB;

	vec2 percent;
	vec3 aux;
	vec3 pointOnUnitCube;

	for (z = 0; z < 6; z++) {

		vec3 faceSide;
		glm_vec3_copy(modelSide[z], faceSide);

		axisA[0] = faceSide[1]; axisA[1] = faceSide[2]; axisA[2] = faceSide[0];

		glm_vec3_cross(faceSide, axisA, axisB);

		for (int y = 0; y < resolution; y++) {
			for (int x = 0; x < resolution; x++) {

				glm_vec3_zero(pointOnUnitCube);
				glm_vec3_zero(aux);

				percent[0] = (float)x / (resolution - 1);
				percent[1] = (float)y / (resolution - 1);

				glm_vec3_scale(axisA, ((percent[0] - 0.5f) * 2), aux);
				glm_vec3_add(aux, pointOnUnitCube, pointOnUnitCube);

				glm_vec3_scale(axisB, ((percent[1] - 0.5f) * 2), aux);
				glm_vec3_add(aux, pointOnUnitCube, pointOnUnitCube);

				glm_vec3_add(faceSide, pointOnUnitCube, pointOnUnitCube);
				glm_vec3_normalize(pointOnUnitCube);

				glm_vec3_copy(pointOnUnitCube, cube->vertices + ((size_t)i * 3));

				if (x != resolution - 1 && y != resolution - 1)
				{
					cube->triangles[t] = i;
					cube->triangles[t + 1] = i + resolution + 1;
					cube->triangles[t + 2] = i + resolution;

					cube->triangles[t + 3] = i;
					cube->triangles[t + 4] = i + 1;
					cube->triangles[t + 5] = i + resolution + 1;

					t += 6;
				}

				i++;
			}
		}
	}
}