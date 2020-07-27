#include "ether.h"

#include <cglm\cglm.h>

using namespace ether;

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
	vao.Add(Vbo(Vbo::Type::Indices, sizeof(indices) / sizeof(indices[0]), indices));
	vao.Add(Vbo(Vbo::Type::Vertices, sizeof(vertices) / sizeof(vertices[0]), vertices));
	vao.Add(Vbo(Vbo::Type::Texture, sizeof(textureCoords) / sizeof(textureCoords[0]), textureCoords, textureLoader.GetId()));
	vao.Load();

	engine.Vaos.push_back(vao);

	engine.Run();

}