#ifndef __ETHER_SHADER_PROGRAM_H__
#define __ETHER_SHADER_PROGRAM_H__

namespace ether {

	class ShaderProgram
	{
	public:
		ShaderProgram();
		~ShaderProgram();

		void Init();
		void Add(const Shader& shader);
		void Use();
		void DontUse();
		
		int  UniformLocation(const std::string& name);
		void Uniform(const std::string& name, bool value);
		void Uniform(const std::string& name, int value);
		void Uniform(const std::string& name, float value);
		void Uniform(const std::string& name, Vector3& value);
		void Uniform(const std::string& name, Matrix4& value);

		void BindAttibute(int index, const std::string& name);

	private:
		void CheckError();

	private:
		unsigned int id;
		std::vector<Shader> shaders;

	};
}

#endif // __ETHER_SHADER_PROGRAM_H__