#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ether.h"

/*
	ObjFile is based on the fast_obj lib C implementation. Link:
	https://github.com/thisistherk/fast_obj
*/

namespace ether {

/* Max supported power when parsing float */
#define MAX_POWER 20

#ifdef _WIN32
	const char ObjFile::pathSeparator = '\\';
	const char ObjFile::otherSep = '/';
#else
	const char ObjFile::pathSeparator = '/';
	const char ObjFile::otherSep = '\\';
#endif

	const double  ObjFile::power10Pos[] = {
		1.0e0,  1.0e1,  1.0e2,  1.0e3,  1.0e4,  1.0e5,  1.0e6,  1.0e7,  1.0e8,  1.0e9,
		1.0e10, 1.0e11, 1.0e12, 1.0e13, 1.0e14, 1.0e15, 1.0e16, 1.0e17, 1.0e18, 1.0e19,
	};

	const double ObjFile::power10Neg[] = {
		1.0e0,   1.0e-1,  1.0e-2,  1.0e-3,  1.0e-4,  1.0e-5,  1.0e-6,  1.0e-7,  1.0e-8,  1.0e-9,
		1.0e-10, 1.0e-11, 1.0e-12, 1.0e-13, 1.0e-14, 1.0e-15, 1.0e-16, 1.0e-17, 1.0e-18, 1.0e-19,
	};

	ObjFile::ObjFile(const std::string& filePath) {
		this->filePath = filePath;

		this->vertexCount = 0;
		this->textcoordCount = 0;
		this->normalCount = 0;
		this->indexCount = 0;

		this->vertexIdx = 0;
		this->textcoordIdx = 0;
		this->normalIdx = 0;
		this->indexIdx = 0;

		this->vertices = 0;
		this->texcoords = 0;
		this->normals = 0;
		//this->faces = 0;
		
		this->lineNumber = 0;
		this->useTextureCoords = false;
	}

	ObjFile::~ObjFile() {
		delete[] vertices;
		delete[] texcoords;
		delete[] normals;
		//delete[] faces;
		delete[] indices;
		delete[] indexedTextcoord;
		delete[] indexedNormal;

		for (auto it = materials.begin(); it != materials.end(); ++it) {
			delete* (it);
		}

		for (auto it = objects.begin(); it != objects.end(); ++it) {
			delete* (it);
		}
	}

	char* ObjFile::LoadFileContent() {
		FILE* f;

		f = fopen(filePath.c_str(), "rb");
		if (!f) {
			throw SystemError("Error while opening file " + filePath);
		}

		long p = ftell(f);
		fseek(f, 0, SEEK_END);
		long n = ftell(f);
		fseek(f, p, SEEK_SET);
		if (n == 0)
			return NULL;

		char* content = new char[n + (size_t)1];

		size_t i = fread(content, 1, n, f);
		content[i] = '\n';
		return content;
	}

	void ObjFile::Load() {
		
		auto file = fopen(filePath.c_str(), "rb");
		if (!file) {
			throw SystemError("Error while opening file " + filePath);
		}

		auto p = ftell(file);
		fseek(file, 0, SEEK_END);
		auto n = ftell(file);
		fseek(file, p, SEEK_SET);
		if (n == 0)
			return;

		std::unique_ptr<char> content(new char[n + (size_t)1]);
		auto buffer = content.get();
		auto read = fread(buffer, 1, n, file);
		fclose(file);
		if (read != n) {
			return;
		}

		buffer[read] = '\0';
		auto end = buffer + read;

		PreProcess(buffer, end);

		if (vertexCount > 0) {
			vertices = new ObjFileVertice[vertexCount];
			indexedTextcoord = new ObjFileVeTextcoord[vertexCount];
			indexedColours = new ObjFileColor[vertexCount];
			indexedNormal = new ObjFileNormal[vertexCount];
			memset(vertices, 0, vertexCount * sizeof(ObjFileVertice));
			memset(indexedTextcoord, 0, vertexCount * sizeof(ObjFileVeTextcoord));
			memset(indexedColours, 0, vertexCount * sizeof(indexedColours));
			memset(indexedNormal, 0, vertexCount * sizeof(ObjFileNormal));
		}

		if (textcoordCount > 0) {
			texcoords = new ObjFileVeTextcoord[textcoordCount];
			memset(texcoords, 0, textcoordCount * sizeof(ObjFileVeTextcoord));
		}

		if (normalCount > 0) {
			normals = new ObjFileNormal[normalCount];
			memset(normals, 0, normalCount * sizeof(ObjFileNormal));
		}

		if (indexCount > 0) {
			//faces = new ObjFileFace[faceCount];
			indices = new ObjFileIndice[indexCount];
			memset(indices, 0, indexCount * sizeof(ObjFileIndice));
		}

		/* Find base path for materials/textures */
		size_t sep = filePath.find_last_of(pathSeparator);
		if (sep != std::string::npos)
			baseDir = filePath.substr(0, sep + 1);

		Process(buffer, end);

		auto empty = std::string();
		FlushObject(empty);
	}

	void ObjFile::PreProcess(const char* ptr, const char* end) {

		if (ptr == NULL || end == NULL)
			return;

		const char* p   = ptr;
		while (p != end) {
			p = SkipWhitespace(p);

			switch (*p) {
			case 'v':
				p++;
				switch (*p++) {
				case ' ':
				case '\t':
					vertexCount++;
					break;
				case 't':
					textcoordCount++;
					break;
				case 'n':
					normalCount++;
					break;
				}
				break;
			case 'f':
				p++;
				switch (*p++) {
				case ' ':
				case '\t':
					indexCount++;
					break;
				}
				break;
			default:
				break;
			}

			p = SkipLine(p);
		}
	}

	void ObjFile::Process(const char* ptr, const char* end){

		if (ptr == NULL || end == NULL)
			return;

		lineNumber = 0;
		const char* p = ptr;
		while (p != end) {

			p = SkipWhitespace(p);

			switch (*p) {
			case 'v':
				p++;
				switch (*p++) {
				case ' ':
				case '\t':
					p = ParseVertex(p);
					break;
				case 't':
					p = ParseTexcoord(p);
					break;
				case 'n':
					p = ParseNormal(p);
					break;
				default:
					p--; /* roll p++ back in case *p was a newline */
				}
				break;
			case 'f':
				p++;
				switch (*p++) {
				case ' ':
				case '\t':
					p = ParseFace(p);
					break;
				default:
					p--; /* roll p++ back in case *p was a newline */
				}
				break;
			case 'o':
				p++;
				switch (*p++) {
				case ' ':
				case '\t':
					p = ParseObject(p);
					break;
				default:
					p--; /* roll p++ back in case *p was a newline */
				}
				break;
			case 'm':
				p++;
				if (p[0] == 't' &&
					p[1] == 'l' &&
					p[2] == 'l' &&
					p[3] == 'i' &&
					p[4] == 'b' &&
					IsWhitespace(p[5]))
					p = ParseMtllib(p + 5);
				break;
			case 'u':
				p++;
				if (p[0] == 's' &&
					p[1] == 'e' &&
					p[2] == 'm' &&
					p[3] == 't' &&
					p[4] == 'l' &&
					IsWhitespace(p[5]))
					p = ParseUsemtl(p + 5);
				break;
			case '#':
				break;
			}

			p = SkipLine(p);
			lineNumber++;
		}
	}

	const char* ObjFile::ParseVertex(const char* ptr) {
		unsigned int ii;
		float        v;

		for (ii = 0; ii < 3; ii++) {
			ptr = ParseFloat(ptr, &v);
			vertices[vertexIdx][ii] = v;
		}

		vertexIdx++;
		return ptr;
	}

	const char* ObjFile::ParseTexcoord(const char* ptr) {
		unsigned int ii;
		float        v;

		for (ii = 0; ii < 2; ii++) {
			ptr = ParseFloat(ptr, &v);
			texcoords[textcoordIdx][ii] = v;
		}

		textcoordIdx++;
		return ptr;
	}

	const char* ObjFile::ParseNormal(const char* ptr) {
		unsigned int ii;
		float        v;

		for (ii = 0; ii < 3; ii++) {
			ptr = ParseFloat(ptr, &v);
			normals[normalIdx][ii] = v;
		}

		normalIdx++;
		return ptr;
	}

	const char* ObjFile::ParseFace(const char* ptr) {
		unsigned int count;
		int p, t, n;

		ptr = SkipWhitespace(ptr);

		count = 0;
		while (!IsNewline(*ptr)) {
			p = t = n = 0;

			if (count > 2) {
				throw EngineError("Polygonal face (more than three vertices) not supported. Line " + std::to_string(lineNumber));
			}

			ptr = ParseInt(ptr, &p);
			if (*ptr == '/') {
				ptr++;
				if (*ptr != '/')
					ptr = ParseInt(ptr, &t);

				if (*ptr == '/') {
					ptr++;
					ptr = ParseInt(ptr, &n);
				}
			}

			p--; t--; n--;

			//faces[faceIdx][count].p = p;
			//faces[faceIdx][count].t = t;
			//faces[faceIdx][count].n = n;

			indices[indexIdx][count] = p;
			
			if (t >= 0 && t < static_cast<int>(textcoordCount)) {
				indexedTextcoord[p][0] = texcoords[t][0];
				indexedTextcoord[p][1] = texcoords[t][1];
			} else if (!objects.empty() && objects.back()->material != NULL) {
				auto material = objects.back()->material;
				indexedColours[p][0] = material->Kd[0];
				indexedColours[p][1] = material->Kd[1];
				indexedColours[p][2] = material->Kd[2];
			}

			if (n >= 0 && n < static_cast<int>(normalCount)) {
				indexedNormal[p][0] = normals[n][0];
				indexedNormal[p][1] = normals[n][1];
				indexedNormal[p][2] = normals[n][2];
			}

			count++;

			ptr = SkipWhitespace(ptr);
		}

		indexIdx++;

		return ptr;
	}

	const char* ObjFile::ParseObject(const char* ptr) {
		const char* s;
		const char* e;

		ptr = SkipWhitespace(ptr);

		s = ptr;
		while (!IsEndOfName(*ptr))
			ptr++;
		e = ptr;

		auto name = std::string(s, (e - s));
		FlushObject(name);

		return ptr;
	}

	void ObjFile::FlushObject(std::string& name) {

		ObjFileGroup* lastObject = NULL;

		if (!objects.empty()) {
			lastObject  = objects.back();
			lastObject->faceCount = indexIdx - lastObject->faceIdx;
		}
		
		if (!name.empty()) {
			ObjFileGroup* obj = new ObjFileGroup();
			obj->name = name;
			obj->faceIdx = indexIdx;
			obj->faceCount = 0;

			objects.push_back(obj);
		}
	}
	
	const char* ObjFile::ParseMtllib(const char* ptr) {
		const char* s;
		const char* p;
		const char* e;
		int found_d;

		s = ptr = SkipWhitespace(ptr);
		while (!IsEndOfName(*ptr))
			ptr++;

		std::string libPath = baseDir + std::string(s, (ptr - s));
		std::replace(libPath.begin(), libPath.end(), otherSep, pathSeparator);

		std::ifstream file{ libPath };
		if (!file) {
			throw EngineError("File " + filePath + " not found or empty");
		}

		std::string line;
		ObjFileMaterial* mtl = new ObjFileMaterial();
		while (std::getline(file, line)) {

			found_d = 0;

			p = SkipWhitespace(line.c_str());

			switch (*p)
			{
			case 'n':
				p++;
				if (p[0] == 'e' &&
					p[1] == 'w' &&
					p[2] == 'm' &&
					p[3] == 't' &&
					p[4] == 'l' &&
					IsWhitespace(p[5])) {

					/* Push previous material (if there is one) */
					if (!mtl->name.empty()) {
						materials.push_back(mtl);
						mtl = new ObjFileMaterial();
					}

					/* Read name */
					p += 5;

					while (IsWhitespace(*p))
						p++;

					s = p;
					while (!IsEndOfName(*p))
						p++;
					e = p;

					mtl->name = std::string(s, (e - s));
				}
				break;

			case 'K':
				if (p[1] == 'a')
					p = ReadMtlTriple(p + 2, mtl->Ka);
				else if (p[1] == 'd')
					p = ReadMtlTriple(p + 2, mtl->Kd);
				else if (p[1] == 's')
					p = ReadMtlTriple(p + 2, mtl->Ks);
				else if (p[1] == 'e')
					p = ReadMtlTriple(p + 2, mtl->Ke);
				else if (p[1] == 't')
					p = ReadMtlTriple(p + 2, mtl->Kt);
				break;

			case 'N':
				if (p[1] == 's')
					p = ReadMtlSingle(p + 2, &mtl->Ns);
				else if (p[1] == 'i')
					p = ReadMtlSingle(p + 2, &mtl->Ni);
				break;

			case 'T':
				if (p[1] == 'r') {
					float Tr;
					p = ReadMtlSingle(p + 2, &Tr); if (!found_d) {
						/* Ignore Tr if we've already read d */
						mtl->d = 1.0f - Tr;
					}
				}
				else if (p[1] == 'f')
					p = ReadMtlTriple(p + 2, mtl->Tf);
				break;

			case 'd':
				if (IsWhitespace(p[1])) {
					p = ReadMtlSingle(p + 1, &mtl->d);
					found_d = 1;
				}
				break;

			case 'i':
				p++;
				if (p[0] == 'l' &&
					p[1] == 'l' &&
					p[2] == 'u' &&
					p[3] == 'm' &&
					IsWhitespace(p[4])) {
					p = ReadMtlInt(p + 4, &mtl->illum);
				}
				break;

			case 'm':
				p++;
				if (p[0] == 'a' &&
					p[1] == 'p' &&
					p[2] == '_') {
					p += 3;
					if (*p == 'K') {
						p++;
						if (IsWhitespace(p[1])) {
							if (*p == 'a')
								p = ReadMap(p + 1, mtl->mapKa);
							else if (*p == 'd')
								p = ReadMap(p + 1, mtl->mapKd);
							else if (*p == 's')
								p = ReadMap(p + 1, mtl->mapKs);
							else if (*p == 'e')
								p = ReadMap(p + 1, mtl->mapKe);
							else if (*p == 't')
								p = ReadMap(p + 1, mtl->mapKt);
						}
					}
					else if (*p == 'N') {
						p++;
						if (IsWhitespace(p[1])) {
							if (*p == 's')
								p = ReadMap(p + 1, mtl->mapNs);
							else if (*p == 'i')
								p = ReadMap(p + 1, mtl->mapNi);
						}
					}
					else if (*p == 'd') {
						p++;
						if (IsWhitespace(*p))
							p = ReadMap(p, mtl->mapD);
					}
					else if (p[0] == 'b' &&
						p[1] == 'u' &&
						p[2] == 'm' &&
						p[3] == 'p' &&
						IsWhitespace(p[4])) {
						p = ReadMap(p + 4, mtl->mapBump);
					}
				}
				break;

			case '#':
				break;
			}
		}

		/* Push final material */
		if (!(mtl->name.empty()))
			materials.push_back(mtl);

		return ptr;
	}


	const char* ObjFile::ReadMtlTriple(const char* p, float v[3]) {
		p = ReadMtlSingle(p, &v[0]);
		p = ReadMtlSingle(p, &v[1]);
		p = ReadMtlSingle(p, &v[2]);

		return p;
	}

	const char* ObjFile::ReadMap(const char* ptr, ObjFileTexture& map) {
		const char* s;
		const char* e;

		ptr = SkipWhitespace(ptr);

		/* Don't support options at present */
		if (*ptr == '-')
			return ptr;

		/* Read name */
		s = ptr;
		while (!IsEndOfName(*ptr))
			ptr++;
		e = ptr;

		map.name = std::string(s, (e - s));
		map.path = baseDir +  std::string(s, (e - s));
		std::replace(map.path.begin(), map.path.end(), otherSep, pathSeparator);

		return e;
	}

	const char* ObjFile::ParseUsemtl(const char* ptr) {
		const char* s;
		const char* e;

		ptr = SkipWhitespace(ptr);

		/* Parse the material name */
		s = ptr;
		while (!IsEndOfName(*ptr))
			ptr++;

		e = ptr;

		if (objects.empty())
			return ptr;

		auto name = std::string(s, (e - s));

		for (auto material : materials) {
			if (!material->name.empty() && material->name == name) {
					objects.back()->material = material;
					break;
			}
		}

		return ptr;
	}

	const char* ObjFile::ParseFloat(const char* ptr, float* val) {
		double        sign;
		double        num;
		double        fra;
		double        div;
		int           eval;
		const double* powers;


		ptr = SkipWhitespace(ptr);

		switch (*ptr) {
		case '+':
			sign = 1.0;
			ptr++;
			break;
		case '-':
			sign = -1.0;
			ptr++;
			break;
		default:
			sign = 1.0;
			break;
		}

		num = 0.0;
		while (IsDigit(*ptr))
			num = 10.0 * num + (double)(*ptr++ - (size_t)'0');

		if (*ptr == '.')
			ptr++;

		fra = 0.0;
		div = 1.0;

		while (IsDigit(*ptr)) {
			fra = 10.0 * fra + (double)(*ptr++ - (size_t)'0');
			div *= 10.0;
		}

		num += fra / div;

		if (IsExponent(*ptr)) {
			ptr++;

			switch (*ptr)
			{
			case '+':
				powers = power10Pos;
				ptr++;
				break;
			case '-':
				powers = power10Neg;
				ptr++;
				break;
			default:
				powers = power10Pos;
				break;
			}

			eval = 0;
			while (IsDigit(*ptr))
				eval = 10 * eval + (*ptr++ - '0');

			num *= (eval >= MAX_POWER) ? 0.0 : powers[eval];
		}

		*val = (float)(sign * num);

		return ptr;
	}

	const char* ObjFile::ParseInt(const char* ptr, int* val) {
		int sign;
		int num;

		if (*ptr == '-') {
			sign = -1;
			ptr++;
		}
		else {
			sign = +1;
		}

		num = 0;
		while (IsDigit(*ptr))
			num = 10 * num + (*ptr++ - '0');

		*val = sign * num;

		return ptr;
	}
}
