#if 0
#include <Ether.h>

/* converting rgb color to opengl format */
//float out = (1.0f / 255) * byte_in;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

using namespace ether;

int main()
{
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

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD\n");
		return -1;
	}

	ShaderProgram  shaderProgram;
	shaderProgram.Init();

	Shader vertexShader("shader\\simple.vs", Shader::Type::Vertex); vertexShader.Load();
	Shader fragmentShader("shader\\simple.fs", Shader::Type::Fragment); fragmentShader.Load();

	shaderProgram.Add(vertexShader);
	shaderProgram.Add(fragmentShader);


	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};


	VertexArray* VAO = new VertexArray();
	VAO->Bind();
	VAO->Add(new VertexBuffer<float>(vertices, sizeof(vertices), 3));
	VAO->AddIndex(new IndexBuffer<unsigned int>(indices, sizeof(indices)));
	VAO->Load();
	VAO->UnBind();
		
	// uncomment this call to draw in wireframe polygons.
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram.Use();

		VAO->Draw();

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);
	//glDeleteProgram(shaderProgram);

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
		glfwSetWindowShouldClose(window, GL_TRUE);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

#else

#include <cglm\cglm.h>

#include "ether.h"

using namespace ether;

void build_planet_mesh(Mesh& mesh, float radius, int resolution);
void noise_settings_configure();

int main(int argc, char* argv[]) {

	auto engine = Engine::getInstance();

	engine->Display.BackGroundColor = "#4a6572";
	engine->Init();

	std::cout << glGetString(GL_VERSION) << std::endl;

	//Shader vertexShader("shader\\texture.vs", Shader::Type::Vertex);
	//Shader fragmentShader("shader\\texture.fs", Shader::Type::Fragment);
	//Shader vertexShader("shader\\color.vs", Shader::Type::Vertex);
	//Shader fragmentShader("shader\\color.fs", Shader::Type::Fragment);
	Shader vertexShader("shader\\simple.vs", Shader::Type::Vertex);
	Shader fragmentShader("shader\\simple.fs", Shader::Type::Fragment);
	vertexShader.Load();
	fragmentShader.Load();

	//engine->ShaderProgram.BindAttibute(0, "position");
	//engine->ShaderProgram.BindAttibute(1, "textureCoordinates");
	//engine->ShaderProgram.BindAttibute(1, "color");
	//engine->ShaderProgram.BindAttibute(2, "normal");

	engine->ShaderProgram.Add(vertexShader);
	engine->ShaderProgram.Add(fragmentShader);

	//Mesh
	//Mesh mesh;
	//build_planet_mesh(mesh, 2.272f, 20);
	//Vao vao;
	//vao.Add(Vbo(Vbo::Type::Indices, mesh.IndicessUnitfCount() , mesh.indices));
	//vao.Add(Vbo(Vbo::Type::Vertices, mesh.VerticesUnitfCount(), mesh.vertices));
	//vao.Load();
	//engine->Vaos.push_back(vao);

	//ObjFile
	//TextureLoader textureLoader("resources\\cube.png");
	//textureLoader.Load();
	//ObjFile objFile("resources\\cube.obj");
	//objFile.Load();

	float vertices[] = {
	 0.5f,  0.5f, 0.0f,  // top right
	 0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f   // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};

	engine->Vao = new VertexArray();
	engine->Vao->Bind();
	engine->Vao->Add(new VertexBuffer<float>(vertices, sizeof(vertices), 3));
	engine->Vao->AddIndex(new IndexBuffer<unsigned int>(indices, sizeof(indices)));
	engine->Vao->Load();
	engine->Vao->UnBind();

	//VertexArray vao;
	//vao.Add(VertexBuffer(VertexBuffer::Type::Indices, objFile.TotalIndices() * 3, objFile.Indices()));
	//vao.Add(VertexBuffer(VertexBuffer::Type::Vertices, objFile.TotalVertex() * 3, objFile.Vertices()));
	//vao.Add(VertexBuffer(VertexBuffer::Type::Texture, objFile.TotalTextcoords() * 2, objFile.Textcoords(), textureLoader.GetId()));
	//vao.Add(Vbo(Vbo::Type::Colours, objFile.TotalColors() * 3, objFile.Colors()));
	//vao.Add(VertexBuffer(VertexBuffer::Type::Normals, objFile.TotalNormals() * 3, objFile.Normals()));
	//vao.Load();
	//engine->Vaos.push_back(vao);

	//Static square
	//Vao vao3;
	//vao.Add(Vbo(Vbo::Type::Indices, sizeof(indices) / sizeof(indices[0]), indices));
	//vao.Add(Vbo(Vbo::Type::Vertices, sizeof(vertices) / sizeof(vertices[0]), vertices));
	//vao.Add(Vbo(Vbo::Type::Texture, sizeof(textureCoords) / sizeof(textureCoords[0]), textureCoords, textureLoader.GetId()));
	//vao3.Load();
	//engine.Vaos.push_back(vao3);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	engine->Run();

}

#define VEC3_BACK     {0,  0, -1}
#define VEC3_DOWN     {0, -1,  0}
#define VEC3_FORWARD  {0,  0,  1}
#define VEC3_LEFT     {-1, 0,  0}
#define VEC3_RIGHT    {1,  0,  0}
#define VEC3_UP       {0,  1,  0}

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))

int   noiseSettings_numLayers = 1;
float noiseSettings_strength = 1;
float noiseSettings_baseRoughness = 1;
float noiseSettings_roughness = 2;
float noiseSettings_persistence = 0.5f;
float noiseSettings_minValue = 0;
Vector3  noiseSettings_centre;

void noise_settings_configure() {
	noiseSettings_strength = 0.24f;
	noiseSettings_numLayers = 5;
	noiseSettings_baseRoughness = 0.91f;
	noiseSettings_roughness = 1.83f;
	noiseSettings_persistence = 0.54f;

	noiseSettings_centre[0] = 2.86f;
	noiseSettings_centre[1] = 1.84f;
	noiseSettings_centre[2] = 2.48f;

	noiseSettings_minValue = 1.0f;
}

float noise_filter_evaluate(SimpleNoise& noise, vec3 vv)
{
	float noiseValue = 0;
	float frequency = noiseSettings_baseRoughness;
	float amplitude = 1;

	Vector3 point(vv[0], vv[1], vv[2] );
	Vector3  vecAux;

	for (int i = 0; i < noiseSettings_numLayers; i++)
	{
		vecAux = point.Scale(frequency, vecAux) + noiseSettings_centre;
		//glm_vec3_scale(point, frequency, vecAux);
		//glm_vec3_add(vecAux, noiseSettings_centre, vecAux);

		float v = noise.Evaluate(vecAux);
		noiseValue += (v + 1) * 0.5f * amplitude;

		frequency *= noiseSettings_roughness;
		amplitude *= noiseSettings_persistence;
	}

	noiseValue = max(0, (noiseValue - noiseSettings_minValue));

	return noiseValue * noiseSettings_strength;
}


void build_planet_mesh(Mesh& mesh, float radius, int resolution)
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

	mesh.totalVertices = 0;
	mesh.totalIndices = 0;

	SimpleNoise noise;
	noise_settings_configure();

	for (z = 0; z < 6; z++) {
		mesh.totalVertices += resolution * resolution;
		mesh.totalIndices += (resolution - 1) * (resolution - 1) * 6;
	}

	mesh.vertices = (float*)malloc(mesh.VerticesSize());
	mesh.indices = (unsigned int*)malloc(mesh.IndicesSize());

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
				float elevation = noise_filter_evaluate(noise, pointOnUnitCube);
				glm_vec3_scale(pointOnUnitCube, (elevation + 1) * radius, pointOnUnitCube);

				/* radius */
				//glm_vec3_scale(pointOnUnitCube, radius, pointOnUnitCube);

				glm_vec3_copy(pointOnUnitCube, mesh.vertices + ((size_t)i * 3));

				if (x != resolution - 1 && y != resolution - 1)
				{
					mesh.indices[t] = i;
					mesh.indices[t + 1] = i + resolution + 1;
					mesh.indices[t + 2] = i + resolution;

					mesh.indices[t + 3] = i;
					mesh.indices[t + 4] = i + 1;
					mesh.indices[t + 5] = i + resolution + 1;

					t += 6;
				}

				i++;
			}
		}
	}
}
#endif