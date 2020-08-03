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
		this->faceCount = 0;

		this->vertexIdx = 0;
		this->textcoordIdx = 0;
		this->normalIdx = 0;
		this->faceIdx = 0;

		this->vertices = 0;
		this->texcoords = 0;
		this->normals = 0;
		this->faces = 0;
		
		this->lineNumber = 0;
	}

	ObjFile::~ObjFile() {
		delete[] vertices;
		delete[] texcoords;
		delete[] normals;
		delete[] faces;
		delete[] verticeIndices;
		delete[] textcoordIndices;
		delete[] normalIndices;

		for (auto it = materials.begin(); it != materials.end(); ++it) {
			delete* (it);
		}

		for (auto it = objects.begin(); it != objects.end(); ++it) {
			delete* (it);
		}
	}

	void ObjFile::Load() {

		PreProcess();

		std::ifstream file{ filePath };

		if (!file) {
			throw EngineError("File " + filePath + " not found or empty");
		}

		if (vertexCount > 0) {
			vertices = new ObjFileVertice[vertexCount];
		}

		if (textcoordCount > 0) {
			texcoords = new ObjFileVeTextcoord[textcoordCount];
		}

		if (normalCount > 0) {
			normals = new ObjFileNormal[normalCount];
		}

		if (faceCount > 0) {
			faces = new ObjFileFace[faceCount];
			verticeIndices = new ObjFileIndice[faceCount];
			textcoordIndices = new ObjFileIndice[faceCount];
			normalIndices = new ObjFileIndice[faceCount];
		}

		/* Find base path for materials/textures */
		size_t sep = filePath.find_last_of(pathSeparator);
		if (sep != std::string::npos)
			baseDir = filePath.substr(0, sep + 1);

		lineNumber = 0;
		std::string line;
		while (std::getline(file, line)) {

			lineNumber++;
			/* Process buffer */
			ParseLine(line);
		}


		auto empty = std::string();
		FlushObject(empty);
	}

	void ObjFile::PreProcess() {

		const char* p   = NULL;
		std::ifstream file{ filePath };

		if (!file) {
			throw EngineError("File " + filePath + " not found or empty");
		}

		std::string line;
		while (std::getline(file, line)) {
			p = SkipWhitespace(line.c_str());

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
					faceCount++;
					break;
				}
				break;
			default:
				break;
			}
		}
	}

	void ObjFile::ParseLine(std::string& line){

		const char* p = SkipWhitespace(line.c_str());

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
			p = 0;
			t = 0;
			n = 0;

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

			faces[faceIdx][count].p = p;
			faces[faceIdx][count].t = t;
			faces[faceIdx][count].n = n;

			verticeIndices[faceIdx][count]   = p;
			textcoordIndices[faceIdx][count] = t;
			normalIndices[faceIdx][count]    = n;

			count++;

			ptr = SkipWhitespace(ptr);
		}

		faceIdx++;

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
			lastObject->faceCount = faceIdx - lastObject->faceIdx;
		}
		
		if (!name.empty()) {
			ObjFileGroup* obj = new ObjFileGroup();
			obj->name = name;
			obj->faceIdx = faceIdx;
			obj->faceCount = 0;

			objects.push_back(obj);
		}
	}
	
	const char* ObjFile::ParseMtllib(const char* ptr) {
		const char* s;
		const char* p;
		const char* e;
		int found_d;

		ptr = SkipWhitespace(ptr);

		std::string libPath = baseDir + ptr;
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
