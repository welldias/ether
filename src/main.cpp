#include "ether.h"

using namespace ether;

int main(int argc, char* argv[]) {

    float vertices[] = {
		-0.5f, 0.5f, 0.0f,//v0
		-0.5f, -0.5f, 0.0f,//v1
		0.5f, -0.5f, 0.0f,//v2
		0.5f, 0.5f, 0.0f,//v3
    };

	int indices[] = {
		0,1,3,//top left triangle (v0, v1, v3)
		3,1,2//bottom right triangle (v3, v1, v2)
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


	Vao vao;
	vao.Add(Vbo(Vbo::Type::Indices, 6, indices));
	vao.Add(Vbo(Vbo::Type::Vertices, 12, vertices));
	vao.Load();

	engine.Vaos.push_back(vao);

	engine.Run();

}