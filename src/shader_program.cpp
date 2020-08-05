#include "ether.h"

namespace ether {

	ShaderProgram::ShaderProgram() {

        this->id = 0;
	}

	ShaderProgram::~ShaderProgram() {
        if (id == 0)
            return;

        DontUse();

        for (auto it = shaders.begin(); it != shaders.end(); it++) {
                glDetachShader(id, (*it).GetId());
        }
		glDeleteProgram(id);
	}

    void ShaderProgram::Init() {
        if (this->id != 0)
            return;

        this->id = glCreateProgram();
    }

	void ShaderProgram::Add(const Shader& shader) {
        glAttachShader(id, shader.GetId());
        glLinkProgram(id);
        CheckError();

        shaders.push_back(shader);
    }

    void ShaderProgram::Use() {
        glUseProgram(id);
    }

    void ShaderProgram::DontUse() {
        glUseProgram(0);
    }

	void ShaderProgram::CheckError() {

        int success = 0;
        char infoLog[512];

        memset(infoLog, 0, sizeof(infoLog));

        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(id, sizeof(infoLog), NULL, infoLog);
			throw EngineError(infoLog);
        }
    }

    void ShaderProgram::BindAttibute(int index, const std::string& name) {
        glBindAttribLocation(id, index, name.c_str());
    }

    int ShaderProgram::UniformLocation(const std::string& name) {
        return glGetUniformLocation(id, name.c_str());
    }

    void ShaderProgram::Uniform(const std::string& name, bool value) {
        glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
    }

    void ShaderProgram::Uniform(const std::string& name, int value) {
        glUniform1i(glGetUniformLocation(id, name.c_str()), value);
    }

    void ShaderProgram::Uniform(const std::string& name, float value) {
        glUniform1f(glGetUniformLocation(id, name.c_str()), value);
    }

    void ShaderProgram::Uniform(const std::string& name, Vector3& value) {
        glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, (float*)value);
    }

    void ShaderProgram::Uniform(const std::string& name, Matrix4& value) {
        glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, (float*)value);
    }

    void ShaderProgram::Uniform(const std::string& name, Rgba& value) {
        glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, (float*)value);
    }
}
