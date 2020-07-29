#ifndef __ETHER_OBJ_FILE_OLD_H__
#define __ETHER_OBJ_FILE_OLD_H__

namespace ether {

	class ObjFile {

	public:
		ObjFile(const std::string& filePath);
		~ObjFile();

		void Load();
		int*   BufferIndices();
		float* BufferVertices();
		float* BufferNormals();
		float* BufferTextures();

		//unsigned int TotalIndices() const { return indices.size() * 9; }
		unsigned int TotalIndices() const { return static_cast<unsigned int>(indices.size() * 3); }
		unsigned int TotalVertices() const { return static_cast<unsigned int>(vertices.size() * 3); }
		unsigned int TotalNormals() const { return static_cast<unsigned int>(normals.size() * 3); }
		unsigned int TotalTextures() const { return static_cast<unsigned int>(textures.size() * 2); }

	private:
		void ThrowInvlidLine(unsigned int lineNumber);

	private:
		
		struct Int9 { int i[9]; };

		std::string path;
		std::vector<Int9> indices;
		std::vector<Vector3> vertices;
		std::vector<Vector3> normals;
		std::vector<Vector2> textures;
	};
}

#endif // __ETHER_OBJ_FILE_OLD_H__