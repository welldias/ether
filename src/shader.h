#ifndef __ETHER_SHADER_H__
#define __ETHER_SHADER_H__

namespace ether {

	class Shader
	{
	public:
		enum class Type {
			Vertex = 0,
			Fragment,
		};

		Shader(const std::string& filePath, Type type);
		~Shader();

		void Load();

		unsigned int GetId() const { return id; }
		Type GetType() const { return type; }
		std::string GetPath() const { return path; }
		std::string GetContet() const { return content; }

	private:
		void CheckError();

	private:
		unsigned int id;
		Type type;
		std::string path;
		std::string content;

	};
}

#endif // __ETHER_SHADER_H__