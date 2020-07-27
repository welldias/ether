#include <cglm\cglm.h>

#include "ether.h"

using namespace ether;

void build_planet_mesh(Mesh& mesh, float radius, int resolution);
void noise_settings_configure();

int main(int argc, char* argv[]) {

	float vertices[] = {
		-0.5f,  0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
    };

	float textureCoords[] = {
		0, 0,
		0, 1,
		1, 1,
		1, 0
	};

	int indices[] = {
		0, 1, 3,
		3, 1, 2
	};

	ether_simple_noise_init(0);
	noise_settings_configure();

	//Mesh cube;
	//build_planet_mesh(cube, 2.272f, 10);

	auto engine = Engine::getInstance();
	engine.Display.BackGroundColor.Set(0.2f, 0.3f, 0.3f);
	engine.Init();

	Shader vertexShader("shader/01.vs", Shader::Type::Vertex);
	Shader fragmentShader("shader/01.fs", Shader::Type::Fragment);
	vertexShader.Load();
	fragmentShader.Load();

	engine.ShaderProgram.Add(vertexShader);
	engine.ShaderProgram.Add(fragmentShader);
	engine.ShaderProgram.BindAttibute(0, "position");
	engine.ShaderProgram.BindAttibute(1, "textureCoordinates");

	TextureLoader textureLoader;
	textureLoader.Load("resources/terrain.jpg");

	Vao vao;
	//vao.Add(Vbo(Vbo::Type::Indices, cube.IndicesSize(), cube.indices));
	//vao.Add(Vbo(Vbo::Type::Vertices, cube.VerticesSize(), cube.vertices));
	vao.Add(Vbo(Vbo::Type::Indices, sizeof(indices) / sizeof(indices[0]), indices));
	vao.Add(Vbo(Vbo::Type::Vertices, sizeof(vertices) / sizeof(vertices[0]), vertices));
	vao.Add(Vbo(Vbo::Type::Texture, sizeof(textureCoords) / sizeof(textureCoords[0]), textureCoords, textureLoader.GetId()));
	vao.Load();

	engine.Vaos.push_back(vao);

	engine.Run();

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
vec3  noiseSettings_centre = { 0, 0, 0 };

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

float noise_filter_evaluate(vec3 point)
{
	float noiseValue = 0;
	float frequency = noiseSettings_baseRoughness;
	float amplitude = 1;

	vec3  vecAux = { 0, 0, 0 };

	for (int i = 0; i < noiseSettings_numLayers; i++)
	{
		glm_vec3_scale(point, frequency, vecAux);
		glm_vec3_add(vecAux, noiseSettings_centre, vecAux);

		float v = ether_simple_noise_evaluate(vecAux);
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

	for (z = 0; z < 6; z++) {
		mesh.totalVertices += resolution * resolution;
		mesh.totalIndices += (resolution - 1) * (resolution - 1) * 6;
	}

	unsigned int ddd1 = sizeof(float) * mesh.totalVertices * 3;
	unsigned int ddd2 = sizeof(int) * mesh.totalIndices;

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
				float elevation = noise_filter_evaluate(pointOnUnitCube);
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