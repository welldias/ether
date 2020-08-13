#if 0
#include <Ether.h>

using namespace ether;

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float deltaTime = 0.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, Camera& camera);

int main()
{
	Display display;
	display.Init();
	display.BackGroundColor = "#4a6572";

	ShaderProgram  shaderProgram;
	shaderProgram.Init();

	//Shader vertexShader("shader\\color.vs", Shader::Type::Vertex);
	//Shader fragmentShader("shader\\color.fs", Shader::Type::Fragment);
	Shader vertexShader("shader\\texture.vs", Shader::Type::Vertex);
	Shader fragmentShader("shader\\texture.fs", Shader::Type::Fragment);
	//Shader vertexShader("shader\\simple.vs", Shader::Type::Vertex);
	//Shader fragmentShader("shader\\simple.fs", Shader::Type::Fragment);

	vertexShader.Load();
	fragmentShader.Load();

	shaderProgram.Add(vertexShader);
	shaderProgram.Add(fragmentShader);

	Light light; 
	light.Position.Set(0, 0, 10);
	light.Color.Set(1, 1, 1);

	Camera camera;
	camera.Configure(0.0f, 0.0f, 7.0f, 0.0f, 1.0f, 0.0f, -90.0f, 0.0f);

	TextureLoader textureLoader("resources\\terrain.jpg");
	textureLoader.Load();

	ObjFile objFile("resources\\cube.obj");
	objFile.Load();

	unsigned int VAO, V_VBO, T_VBO, N_VBO, EBO;
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	glGenBuffers(1, &V_VBO);
	// Generate and populate the buffers with vertex attributes and the indices
	glBindBuffer(GL_ARRAY_BUFFER, V_VBO);
	glBufferData(GL_ARRAY_BUFFER, objFile.SizeVertices(), objFile.Vertices(), GL_STATIC_DRAW);

	glGenBuffers(1, &T_VBO);
	//glBindBuffer(GL_ARRAY_BUFFER, T_VBO);
	//glBufferData(GL_ARRAY_BUFFER, objFile.SizeColors(), objFile.Colors(), GL_STATIC_DRAW);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, T_VBO);
	glBufferData(GL_ARRAY_BUFFER, objFile.SizeTextcoords(), objFile.Textcoords(), GL_STATIC_DRAW);

	glGenBuffers(1, &N_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, N_VBO);
	glBufferData(GL_ARRAY_BUFFER, objFile.SizeNormals(), objFile.Normals(), GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, objFile.SizeIndices(), objFile.Indices(), GL_STATIC_DRAW);
	
	glBindVertexArray(0);

	glBindVertexArray(VAO);
	
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBindBuffer(GL_ARRAY_BUFFER, V_VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, T_VBO);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, N_VBO);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);


	shaderProgram.BindAttibute(0, "position");
	shaderProgram.BindAttibute(1, "textureCoordinates");
	//shaderProgram.BindAttibute(1, "color");
	shaderProgram.BindAttibute(2, "normal");

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	float startTime = 0;
	float lastTime = (float)glfwGetTime();
	float passedTime = 0;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureLoader.GetId());

	while (!display.ShouldClose()) {

		startTime = static_cast<float>(glfwGetTime());
		deltaTime = passedTime = startTime - lastTime;
		lastTime = startTime;


		processInput(display.getWindowHandle(), camera);

		display.Clear();

		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, textureLoader.GetId());
		
		shaderProgram.Use();

		shaderProgram.Uniform("lightPosition", light.Position);
		shaderProgram.Uniform("lightColour", light.Color);

		Matrix4 projection;
		camera.Perspective((float)display.Width / (float)display.Height, 0.1f, 100.0f, projection);
		shaderProgram.Uniform("projection", projection);
		
		Matrix4 view;
		camera.ViewMatrix(view);
		shaderProgram.Uniform("view", view);

		Matrix4 model;
		//Vector3 vecA(0.0f, 0.0f, 0.0f);
		//Vector3 vecB(0.0f, -0.05f, 0.0f);
		model.Identity();
		//model.Translate(vecA);
		//model.Rotate(MathUtil::Radian(10.0f * static_cast<float>(glfwGetTime())), vecB);
		shaderProgram.Uniform("model", model);

		//VAO->Draw();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(display.getWindowHandle());
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &V_VBO);
	glDeleteBuffers(1, &EBO);
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

void processInput(GLFWwindow* window, Camera& camera)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		camera.MoveBackward((float)2.5 * deltaTime);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		camera.MoveForward((float)2.5 * deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		camera.MoveLeft((float)2.5 * deltaTime);
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		camera.MoveRight((float)2.5 * deltaTime);
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

	Shader vertexShader("shader\\texture.vs", Shader::Type::Vertex);
	Shader fragmentShader("shader\\texture.fs", Shader::Type::Fragment);
	//Shader vertexShader("shader\\color.vs", Shader::Type::Vertex);
	//Shader fragmentShader("shader\\color.fs", Shader::Type::Fragment);
	//Shader vertexShader("shader\\simple.vs", Shader::Type::Vertex);
	//Shader fragmentShader("shader\\simple.fs", Shader::Type::Fragment);
	vertexShader.Load();
	fragmentShader.Load();

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

	TextureLoader textureLoader("resources\\terrain.jpg");
	textureLoader.Load();

	ObjFile objFile("resources\\cube.obj");
	objFile.Load();

	engine->Vao = new VertexArray();
	engine->Vao->Bind();
	engine->Vao->Add(new VertexBuffer<float>(objFile.Vertices(), objFile.SizeVertices(), 3));
	engine->Vao->Add(new VertexBuffer<float>(objFile.Textcoords(), objFile.SizeTextcoords(), 2));
	//engine->Vao->Add(new VertexBuffer<float>(objFile.Colors(), objFile.SizeColors(), 3));
	engine->Vao->Add(new VertexBuffer<float>(objFile.Normals(), objFile.SizeNormals(), 3));
	engine->Vao->AddIndex(new IndexBuffer<unsigned int>(objFile.Indices(), objFile.SizeIndices()));
	engine->Vao->UnBind();

	engine->Vao->Load();

	engine->ShaderProgram.BindAttibute(0, "position");
	engine->ShaderProgram.BindAttibute(1, "textureCoordinates");
	//engine->ShaderProgram.BindAttibute(1, "color");
	engine->ShaderProgram.BindAttibute(2, "normal");

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureLoader.GetId());

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