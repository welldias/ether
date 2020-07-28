#include "ether.h"

namespace ether {

	ObjFile::ObjFile(const std::string& filePath) {
		this->path = filePath;
	}

	ObjFile::~ObjFile() {

	}

	void ObjFile::Load() {

		std::ifstream file{ path };

		if (!file) {
			throw EngineError("File " + path + " not found or empty");
		}

		unsigned int lineNumber = 0;
		std::string line;

		std::string lineType;
		float f1, f2, f3;
		std::string sf1, sf2, sf3;

		std::string token;
		Int9 int9;

		while (std::getline(file, line))
		{
			if (line[0] == '#' || line.empty())
				continue;

			std::istringstream iss(line);

			if (line.substr(0, 3) == "vn ") {
				if (!(iss >> lineType >> f1 >> f2 >> f3))
					ThrowInvlidLine(lineNumber);
				normals.push_back(Vector3(f1, f2, f3));
			}
			else if (line.substr(0, 3) == "vt ") {
				if (!(iss >> lineType >> f1 >> f2))
					ThrowInvlidLine(lineNumber);
				textures.push_back(Vector2(f1, f2));
			}
			else if (line.substr(0, 2) == "v ") {
				if (!(iss >> lineType >> f1 >> f2 >> f3))
					ThrowInvlidLine(lineNumber);
				vertices.push_back(Vector3(f1, f2, f3));
			}
			else if (line.substr(0, 2) == "f ") {
				if (!(iss >> lineType >> sf1 >> sf2 >> sf3))
					ThrowInvlidLine(lineNumber);

				std::stringstream ss1(sf1);
				for (int i = 0; i < 3; i++) {
					if (!std::getline(ss1, token, '/'))
						ThrowInvlidLine(lineNumber);
					int9.i[i] = std::stoi(token);
				}

				std::stringstream ss2(sf2);
				for (int i = 0; i < 3; i++) {
					if (!std::getline(ss2, token, '/'))
						ThrowInvlidLine(lineNumber);
					int9.i[i+3] = std::stoi(token);
				}

				std::stringstream ss3(sf3);
				for (int i = 0; i < 3; i++) {
					if (!std::getline(ss3, token, '/'))
						ThrowInvlidLine(lineNumber);
					int9.i[i + 6] = std::stoi(token);
				}

				indices.push_back(int9);
			}

		}
	}

	int* ObjFile::BufferIndices() {
		if (indices.size() == 0)
			return NULL;

		//int* buffer = (int*)malloc(indices.size() * sizeof(int) * 9);
		int* buffer = (int*)malloc(indices.size() * sizeof(int) * 3);
		if (buffer == 0)
			return NULL;

		unsigned i = 0;
		for (auto it = indices.begin(); it != indices.end(); it++) {
			buffer[i++] = (*it).i[0];
			buffer[i++] = (*it).i[3];
			buffer[i++] = (*it).i[6];

			//buffer[i++] = (*it).i[3];
			//buffer[i++] = (*it).i[4];
			//buffer[i++] = (*it).i[5];
			//
			//buffer[i++] = (*it).i[6];
			//buffer[i++] = (*it).i[7];
			//buffer[i++] = (*it).i[8];
		}

		return buffer;
	}

	float* ObjFile::BufferVertices() {
		if (vertices.size() == 0)
			return NULL;

		float* buffer = (float*)malloc(vertices.size() * sizeof(float) * 3);
		if (buffer == 0)
			return NULL;

		unsigned i = 0;
		for (auto it = vertices.begin(); it != vertices.end(); it++) {
			buffer[i++] = (*it).v[0];
			buffer[i++] = (*it).v[1];
			buffer[i++] = (*it).v[2];
		}

		return buffer;
	}
	
	float* ObjFile::BufferNormals() {
		if (normals.size() == 0)
			return NULL;

		float* buffer = (float*)malloc(normals.size() * sizeof(float) * 3);
		if (buffer == 0)
			return NULL;

		unsigned i = 0;
		for (auto it = normals.begin(); it != normals.end(); it++) {
			buffer[i++] = (*it).v[0];
			buffer[i++] = (*it).v[1];
			buffer[i++] = (*it).v[2];
		}

		return buffer;
	}

	float* ObjFile::BufferTextures() {
		if (textures.size() == 0)
			return NULL;

		float* buffer = (float*)malloc(textures.size() * sizeof(float) * 2);
		if (buffer == 0)
			return NULL;

		unsigned i = 0;
		for (auto it = textures.begin(); it != textures.end(); it++) {
			buffer[i++] = (*it).v[0];
			buffer[i++] = (*it).v[1];
		}

		return buffer;
	}


	void ObjFile::ThrowInvlidLine(unsigned int lineNumber) {
		throw EngineError("Syntax error file " + path + ", line " + std::to_string(lineNumber));
	}

}
