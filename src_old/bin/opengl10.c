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

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void build_planet_mesh(EtherMesh* mesh, float radius, int resolution);

void noise_settings_configure();

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
	ether_simple_noise_init(0);
	noise_settings_configure();

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

	ether_camera_gl_init(&camera);
	ether_camera_gl_configure(&camera, 0.0f, 0.0f, 7.5f, 0.0f, 1.0f, 0.0f, -90.0f, 0.0f);

	EtherMesh cube;
	ether_mesh_init(&cube);
	build_planet_mesh(&cube, 2.272f, 50);

	EtherVbo vbo;
	ether_vbo_init(&vbo, ETHER_VBO_TYPE_MESH, &cube);
	ether_vbo_gpu_store(&vbo);

	// uncomment this call to draw in wireframe polygons.
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	ether_timer_init();

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

		mat4 model = GLM_MAT4_IDENTITY_INIT;
		glm_translate(model, (vec3) { 0.0f, 0.0f, 0.0f });
		glm_rotate(model, glm_rad(20.0f), (vec3){ 1.0f, 0.3f, 0.5f });
		ether_shader_uniform_mat4(shaderId, "model", model);

		ether_vbo_draw(&vbo, shaderId);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();

		ether_timer_delay(10);
	}

	ether_vbo_relesase(&vbo);
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

int   noiseSettings_numLayers = 1;
float noiseSettings_strength  = 1;
float noiseSettings_baseRoughness = 1;
float noiseSettings_roughness = 2;
float noiseSettings_persistence = 0.5f;
float noiseSettings_minValue = 0;
vec3  noiseSettings_centre = { 0, 0, 0 };

void noise_settings_configure() {
	noiseSettings_strength  = 0.24f;
	noiseSettings_numLayers = 5;
	noiseSettings_baseRoughness = 0.91f;
	noiseSettings_roughness = 1.83f;
	noiseSettings_persistence = 0.54f;

	noiseSettings_centre[0] = 2.86f;
	noiseSettings_centre[1] = 1.84f;
	noiseSettings_centre[2] = 2.48f;

	noiseSettings_minValue = 1.0f;
}

float noise_filter_evaluate(vec3 point)
{
	float noiseValue = 0;
	float frequency = noiseSettings_baseRoughness;
	float amplitude = 1;

	vec3  vecAux = {0, 0, 0};

	for (int i = 0; i < noiseSettings_numLayers; i++)
	{
		glm_vec3_scale(point, frequency, vecAux);
		glm_vec3_add(vecAux, noiseSettings_centre, vecAux);

		float v = ether_simple_noise_evaluate(vecAux);
		noiseValue += (v + 1) * 0.5f * amplitude;
		
		frequency *= noiseSettings_roughness;
		amplitude *= noiseSettings_persistence;
	}

	noiseValue =  max(0, (noiseValue - noiseSettings_minValue));

	return noiseValue * noiseSettings_strength;
}

void build_planet_mesh(EtherMesh* mesh, float radius, int resolution)
{
	vec3 modelSide[] = {
		VEC3_UP,
		VEC3_FORWARD,
		VEC3_DOWN,
		VEC3_BACK,
		VEC3_LEFT,
		VEC3_RIGHT,
	};

	int i = 0;
	int t = 0;
	int z = 0;

	vec3 axisA;
	vec3 axisB;

	vec2 percent;
	vec3 aux;
	vec3 pointOnUnitCube;

	mesh->totalVertices  = 0;
	mesh->totalIndices   = 0;
	

	for (z = 0; z < 6; z++) {
		mesh->totalVertices  += resolution * resolution;
		mesh->totalIndices += (resolution - 1) * (resolution - 1) * 6;
	}

	unsigned int ddd1 = sizeof(float) * mesh->totalVertices * 3;
	unsigned int ddd2 = sizeof(int) * mesh->totalIndices;

	mesh->vertices = (float*)malloc(ether_mesh_vertices_size(mesh));
	mesh->indices  = (unsigned int*)malloc(ether_mesh_indices_size(mesh));

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
				
				/*noise elevation*/
				float elevation = noise_filter_evaluate(pointOnUnitCube);
				glm_vec3_scale(pointOnUnitCube, (elevation+1) * radius, pointOnUnitCube);

				/* radius */
				//glm_vec3_scale(pointOnUnitCube, radius, pointOnUnitCube);

				glm_vec3_copy(pointOnUnitCube, mesh->vertices + ((size_t)i * 3));

				if (x != resolution - 1 && y != resolution - 1)
				{
					mesh->indices[t] = i;
					mesh->indices[t + 1] = i + resolution + 1;
					mesh->indices[t + 2] = i + resolution;

					mesh->indices[t + 3] = i;
					mesh->indices[t + 4] = i + 1;
					mesh->indices[t + 5] = i + resolution + 1;

					t += 6;
				}

				i++;
			}
		}
	}
}