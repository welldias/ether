#include "ether.h"

namespace ether {

	Shader::Shader(const std::string& filePath, Type type) {

		this->id = 0;
		this->path = filePath;
		this->type = type;
	}

	Shader::~Shader() {
		if(id != 0)
			glDeleteShader(id);
	}

	void Shader::Load() {

		std::ifstream file{ path };

		if (!file) {
			content = {};
			throw EngineError("File "+ path +" not found or empty");
		}
		
		content = std::string{
			std::istreambuf_iterator<char>{ file },
			std::istreambuf_iterator<char>()
		};

		switch (type)
		{
		case Type::Vertex:
			id = glCreateShader(GL_VERTEX_SHADER);
			break;
		case Type::Fragment:
			id = glCreateShader(GL_FRAGMENT_SHADER);
			break;
		default:
			throw EngineError("Shader type undefined ");
			break;
		}

		const char* ptrContent = content.c_str();
		int size = static_cast<int>(content.length());
		
		glShaderSource(id, 1, (const GLchar* const*)&ptrContent, &size);
		glCompileShader(id);
		CheckError();
	}

    void Shader::CheckError() {

        int success = 0;
        char infoLog[512];

        memset(infoLog, 0, sizeof(infoLog));

        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(id, sizeof(infoLog), NULL, infoLog);
			throw EngineError(infoLog);
        }
    }

}
