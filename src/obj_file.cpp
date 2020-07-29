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
	/* Size of buffer to read into */
	#define BUFFER_SIZE 65536

#ifdef _WIN32
	const char ObjFile::pathSeparator  = '\\';
	const char ObjFile::otherSep       = '/';
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

#define array_clean(_arr)       ((_arr) ? free(_array_header(_arr)), 0 : 0)
#define array_push(_arr, _val)  (_array_mgrow(_arr, 1) ? ((_arr)[_array_size(_arr)++] = (_val), _array_size(_arr) - 1) : 0)
#define array_size(_arr)        ((_arr) ? _array_size(_arr) : 0)
#define array_capacity(_arr)    ((_arr) ? _array_capacity(_arr) : 0)
#define array_empty(_arr)       (array_size(_arr) == 0)

#define _array_header(_arr)     ((ObjFileUnit*)(_arr)-2)
#define _array_size(_arr)       (_array_header(_arr)[0])
#define _array_capacity(_arr)   (_array_header(_arr)[1])
#define _array_ngrow(_arr, _n)  ((_arr) == 0 || (_array_size(_arr) + (_n) >= _array_capacity(_arr)))
#define _array_mgrow(_arr, _n)  (_array_ngrow(_arr, _n) ? (_array_grow(_arr, _n) != 0) : 1)
#define _array_grow(_arr, _n)   (*((void**)&(_arr)) = ArrayRealloc(_arr, _n, sizeof(*(_arr))))


	ObjFile::ObjFile(const std::string& filePath) {
		this->filePath = filePath;

		this->positions = 0;
		this->texcoords = 0;
		this->normals = 0;
		this->face_vertices = 0;
		this->face_materials = 0;
		this->indices = 0;
	}

	ObjFile::~ObjFile() {
		for (auto it = groups.begin(); it != groups.end(); ++it) {
			delete *(it);
		}

		for (auto it = materials.begin(); it != materials.end(); ++it) {
			delete* (it);
		}

		free(positions);
		free(texcoords);
		free(normals);
		free(face_vertices);
		free(face_materials);
		free(indices);
	}

	void ObjFile::Load() {

		ObjFileData  data;

		FILE* file;
		char* buffer;
		char* start;
		char* end;
		char* last;
		ObjFileUnit  read;
		size_t       sep;
		ObjFileUnit  bytes;

		/* Open file */
		file = fopen(filePath.c_str(), "rb");
		if (!file) {
			throw SystemError("Erro opening file " + filePath );
		}

		/* Add dummy position/texcoord/normal */
		array_push(this->positions, 0.0f);
		array_push(this->positions, 0.0f);
		array_push(this->positions, 0.0f);

		array_push(this->texcoords, 0.0f);
		array_push(this->texcoords, 0.0f);

		array_push(this->normals, 0.0f);
		array_push(this->normals, 0.0f);
		array_push(this->normals, 1.0f);

		/* Data needed during parsing */
		data.mesh = this;
		GroupDefault(data.group);
		data.material = 0;
		data.line = 1;
		data.base = 0;

		/* Find base path for materials/textures */
		if (StringFindLast(filePath.c_str(), ObjFile::pathSeparator, &sep))
			data.base = StringSubstr(filePath.c_str(), 0, sep + 1);

		/* Create buffer for reading file */
		buffer = (char*)(realloc(0, 2 * BUFFER_SIZE * sizeof(char)));
		if (!buffer) {
			throw SystemError("Not enough memory space");
		}

		start = buffer;
		for (;;) {
			/* Read another buffer's worth from file */
			read = (ObjFileUnit)(fread(start, 1, BUFFER_SIZE, file));
			if (read == 0 && start == buffer)
				break;

			/* Ensure buffer ends in a newline */
			if (read < BUFFER_SIZE) {
				if (read == 0 || start[read - 1] != '\n')
					start[read++] = '\n';
			}

			end = start + read;
			if (end == buffer)
				break;

			/* Find last new line */
			last = end;
			while (last > buffer)
			{
				last--;
				if (*last == '\n')
					break;
			}

			/* Check there actually is a new line */
			if (*last != '\n')
				break;

			last++;

			/* Process buffer */
			ParseBuffer(data, buffer, last);

			/* Copy overflow for next buffer */
			bytes = (ObjFileUnit)(end - last);
			memmove(buffer, last, bytes);
			start = buffer + bytes;
		}

		/* Flush final group */
		FlushOutput(data);

		position_count = array_size(positions) / 3;
		texcoord_count = array_size(texcoords) / 2;
		normal_count   = array_size(normals) / 3;
		face_count     = array_size(face_vertices);

		/* Clean up */
		free(buffer);
		free(data.base);

		fclose(file);
	}

	void ObjFile::ParseBuffer(ObjFileData& data, const char* ptr, const char* end) {

		const char* p = ptr;
		while (p != end) {
			p = SkipWhitespace(p);

			switch (*p) {
			case 'v':
				p++;
				switch (*p++) {
				case ' ':
				case '\t':
					p = ParseVertex(data, p);
					break;
				case 't':
					p = ParseTexcoord(data, p);
					break;
				case 'n':
					p = ParseNormal(data, p);
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
					p = ParseFace(data, p);
					break;
				default:
					p--; /* roll p++ back in case *p was a newline */
				}
				break;
			case 'g':
				p++;
				switch (*p++) {
				case ' ':
				case '\t':
					p = ParseGroup(data, p);
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
					p = ParseMtllib(data, p + 5);
				break;
			case 'u':
				p++;
				if (p[0] == 's' &&
					p[1] == 'e' &&
					p[2] == 'm' &&
					p[3] == 't' &&
					p[4] == 'l' &&
					IsWhitespace(p[5]))
					p = ParseUsemtl(data, p + 5);
				break;
			case '#':
				break;
			}

			p = SkipLine(p);

			data.line++;
		}
	}

	void ObjFile::FlushOutput(ObjFileData& data) {
		/* Add group if not empty */
		if (data.group.face_count > 0) {
			ObjFileGroup* gr = new ObjFileGroup();
			gr->name = data.group.name;
			gr->face_count = data.group.face_count;
			gr->face_offset = data.group.face_offset;
			gr->index_offset = data.group.index_offset;
			data.mesh->groups.push_back(gr);
		}
		//or group_clean(data.group);

		/* Reset for more data */
		GroupDefault(data.group);
		data.group.face_offset = array_size(data.mesh->face_vertices);
		data.group.index_offset = array_size(data.mesh->indices);
	}

	void ObjFile::GroupDefault(ObjFileGroup& group) {
		group.face_count = 0;
		group.face_offset = 0;
		group.index_offset = 0;
	}

	const char* ObjFile::ParseVertex(ObjFileData& data, const char* ptr) {
		
		unsigned int ii;
		float        v;

		for (ii = 0; ii < 3; ii++) {
			ptr = ParseFloat(ptr, &v);
			array_push(data.mesh->positions, v);
		}

		return ptr;
	}

	const char* ObjFile::ParseTexcoord(ObjFileData& data, const char* ptr) {
		unsigned int ii;
		float        v;

		for (ii = 0; ii < 2; ii++) {
			ptr = ParseFloat(ptr, &v);
			array_push(data.mesh->texcoords, v);
		}

		return ptr;
	}

	const char* ObjFile::ParseNormal(ObjFileData& data, const char* ptr) {
		unsigned int ii;
		float        v;

		for (ii = 0; ii < 3; ii++) {
			ptr = ParseFloat(ptr, &v);
			array_push(data.mesh->normals, v);
		}

		return ptr;
	}

	const char* ObjFile::ParseFace(ObjFileData& data, const char* ptr) {
		unsigned int count;
		ObjFileIndex vn;
		int          v;
		int          t;
		int          n;

		ptr = SkipWhitespace(ptr);

		count = 0;
		while (!IsNewline(*ptr)) {
			v = 0;
			t = 0;
			n = 0;

			ptr = ParseInt(ptr, &v);
			if (*ptr == '/') {
				ptr++;
				if (*ptr != '/')
					ptr = ParseInt(ptr, &t);

				if (*ptr == '/') {
					ptr++;
					ptr = ParseInt(ptr, &n);
				}
			}

			if (v < 0)
				vn.p = (array_size(data.mesh->positions) / 3) - (ObjFileUnit)(-v);
			else
				vn.p = (ObjFileUnit)(v);

			if (t < 0)
				vn.t = (array_size(data.mesh->texcoords) / 2) - (ObjFileUnit)(-t);
			else if (t > 0)
				vn.t = (ObjFileUnit)(t);
			else
				vn.t = 0;

			if (n < 0)
				vn.n = (array_size(data.mesh->normals) / 3) - (ObjFileUnit)(-n);
			else if (n > 0)
				vn.n = (ObjFileUnit)(n);
			else
				vn.n = 0;

			array_push(data.mesh->indices, vn);
			count++;

			ptr = SkipWhitespace(ptr);
		}

		array_push(data.mesh->face_vertices, count);
		array_push(data.mesh->face_materials, data.material);

		data.group.face_count++;

		return ptr;
	}

	const char* ObjFile::ParseGroup(ObjFileData& data, const char* ptr) {
		const char* s;
		const char* e;

		ptr = SkipWhitespace(ptr);

		s = ptr;
		while (!IsEndOfName(*ptr))
			ptr++;

		e = ptr;

		FlushOutput(data);
		data.group.name = StringCopy(s, e);

		return ptr;
	}

	const char* ObjFile::ParseMtllib(ObjFileData& data, const char* ptr) {
		const char* s;
		const char* e;
		char* lib;
		FILE* file;


		ptr = SkipWhitespace(ptr);

		s = ptr;
		while (!IsEndOfName(*ptr))
			ptr++;

		e = ptr;

		lib = StringConcat(data.base, s, e);
		if (lib) {
			StringFixSeparators(lib);

			file = fopen(lib, "rb");
			if (file) {
				ReadMtllib(data, file);
				fclose(file);
			}

			free(lib);
		}

		return ptr;
	}

	const char* ObjFile::ParseUsemtl(ObjFileData& data, const char* ptr) {
		const char* s;
		const char* e;
		unsigned int     idx;
		ObjFileMaterial* mtl;

		ptr = SkipWhitespace(ptr);

		/* Parse the material name */
		s = ptr;
		while (!IsEndOfName(*ptr))
			ptr++;

		e = ptr;

		/* If there are no materials yet, add a dummy invalid material at index 0 */
		if (data.mesh->materials.empty())
			data.mesh->materials.push_back(MtlDefault());

		/* Find an existing material with the same name */
		idx = 0;
		while (idx < data.mesh->materials.size()) {
			mtl = data.mesh->materials[idx];
			if (!mtl->name.empty() && StringEqual(mtl->name.c_str(), s, e))
				break;

			idx++;
		}

		if (idx == data.mesh->materials.size())
			idx = 0;

		data.material = idx;

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
		} else {
			sign = +1;
		}

		num = 0;
		while (IsDigit(*ptr))
			num = 10 * num + (*ptr++ - '0');

		*val = sign * num;

		return ptr;
	}

	void* ObjFile::ArrayRealloc(void* ptr, ObjFileUnit n, ObjFileUnit b) {
		ObjFileUnit sz = array_size(ptr);
		ObjFileUnit nsz = sz + n;
		ObjFileUnit cap = array_capacity(ptr);
		ObjFileUnit ncap = 3 * cap / 2;
		ObjFileUnit* r;


		if (ncap < nsz)
			ncap = nsz;
		ncap = (ncap + 15) & ~15u;

		r = (ObjFileUnit*)(realloc(ptr ? _array_header(ptr) : 0, b * (size_t)ncap + 2 * sizeof(ObjFileUnit)));
		if (!r)
			return 0;

		r[0] = sz;
		r[1] = ncap;

		return (r + 2);
	}

	unsigned long ObjFile::FileSize(void* file)
	{
		FILE* f;
		long p;
		long n;

		f = (FILE*)(file);

		p = ftell(f);
		fseek(f, 0, SEEK_END);
		n = ftell(f);
		fseek(f, p, SEEK_SET);

		if (n > 0)
			return (unsigned long)(n);
		else
			return 0;
	}

	char* ObjFile::StringCopy(const char* s, const char* e) {
		size_t n;
		char* p;

		n = (size_t)(e - s);
		p = (char*)(realloc(0, n + 1));
		if (p) {
			memcpy(p, s, n);
			p[n] = '\0';
		}

		return p;
	}

	char* ObjFile::StringSubstr(const char* s, size_t a, size_t b) {
		return StringCopy(s + a, s + b);
	}

	char* ObjFile::StringConcat(const char* a, const char* s, const char* e) {
		size_t an;
		size_t sn;
		char* p;

		an = a ? strlen(a) : 0;
		sn = (size_t)(e - s);
		p = (char*)(realloc(0, an + sn + 1));
		if (p) {
			if (a)
				memcpy(p, a, an);
			memcpy(p + an, s, sn);
			p[an + sn] = '\0';
		}

		return p;
	}

	int ObjFile::StringEqual(const char* a, const char* s, const char* e) {
		size_t an = strlen(a);
		size_t sn = (size_t)(e - s);

		return an == sn && memcmp(a, s, an) == 0;
	}

	int ObjFile::StringFindLast(const char* s, char c, size_t* p) {
		const char* e;

		e = s + strlen(s);
		while (e > s) {
			e--;

			if (*e == c) {
				*p = (size_t)(e - s);
				return 1;
			}
		}

		return 0;
	}

	void ObjFile::StringFixSeparators(char* s) {
		while (*s) {
			if (*s == otherSep)
				*s = pathSeparator;
			s++;
		}
	}

	ObjFileMaterial* ObjFile::MtlDefault(void)
	{
		ObjFileMaterial* mtl = new ObjFileMaterial();

		mtl->Ka[0] = 0.0;
		mtl->Ka[1] = 0.0;
		mtl->Ka[2] = 0.0;
		mtl->Kd[0] = 1.0;
		mtl->Kd[1] = 1.0;
		mtl->Kd[2] = 1.0;
		mtl->Ks[0] = 0.0;
		mtl->Ks[1] = 0.0;
		mtl->Ks[2] = 0.0;
		mtl->Ke[0] = 0.0;
		mtl->Ke[1] = 0.0;
		mtl->Ke[2] = 0.0;
		mtl->Kt[0] = 0.0;
		mtl->Kt[1] = 0.0;
		mtl->Kt[2] = 0.0;
		mtl->Ns = 1.0;
		mtl->Ni = 1.0;
		mtl->Tf[0] = 1.0;
		mtl->Tf[1] = 1.0;
		mtl->Tf[2] = 1.0;
		mtl->d = 1.0;
		mtl->illum = 1;

		return mtl;
	}

	const char* ObjFile::ReadMtlTriple(const char* p, float v[3]) {
		p = ReadMtlSingle(p, &v[0]);
		p = ReadMtlSingle(p, &v[1]);
		p = ReadMtlSingle(p, &v[2]);
		return p;
	}

	const char* ObjFile::ReadMap(ObjFileData& data, const char* ptr, ObjFileTexture& map) {
		const char* s;
		const char* e;
		char* name;
		char* path;

		ptr = SkipWhitespace(ptr);

		/* Don't support options at present */
		if (*ptr == '-')
			return ptr;


		/* Read name */
		s = ptr;
		while (!IsEndOfName(*ptr))
			ptr++;

		e = ptr;

		name = StringCopy(s, e);

		path = StringConcat(data.base, s, e);
		StringFixSeparators(path);

		map.name = name;
		map.path = path;

		return e;
	}


	int ObjFile::ReadMtllib(ObjFileData& data, FILE* file)
	{
		unsigned long   n;
		const char* s;
		char* contents;
		size_t          l;
		const char* p;
		const char* e;
		int             found_d;
		ObjFileMaterial* mtl;


		/* Read entire file */
		n = FileSize(file);

		contents = (char*)(realloc(0, n + (size_t)1));
		if (!contents)
			return 0;

		l = fread(contents, 1, n, file);
		contents[l] = '\n';

		mtl = MtlDefault();

		found_d = 0;

		p = contents;
		e = contents + l;
		while (p < e)
		{
			p = SkipWhitespace(p);

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
						data.mesh->materials.push_back(mtl);
						mtl = MtlDefault();
					}


					/* Read name */
					p += 5;

					while (IsWhitespace(*p))
						p++;

					s = p;
					while (!IsEndOfName(*p))
						p++;

					mtl->name = StringCopy(s, p);
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
					p = ReadMtlSingle(p + 2, &Tr); if (!found_d)
					{
						/* Ignore Tr if we've already read d */
						mtl->d = 1.0f - Tr;
					}
				}
				else if (p[1] == 'f')
					p = ReadMtlTriple(p + 2, mtl->Tf);
				break;

			case 'd':
				if (IsWhitespace(p[1]))
				{
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
								p = ReadMap(data, p + 1, mtl->map_Ka);
							else if (*p == 'd')
								p = ReadMap(data, p + 1, mtl->map_Kd);
							else if (*p == 's')
								p = ReadMap(data, p + 1, mtl->map_Ks);
							else if (*p == 'e')
								p = ReadMap(data, p + 1, mtl->map_Ke);
							else if (*p == 't')
								p = ReadMap(data, p + 1, mtl->map_Kt);
						}
					} 
					else if (*p == 'N') {
						p++;
						if (IsWhitespace(p[1])) {
							if (*p == 's')
								p = ReadMap(data, p + 1, mtl->map_Ns);
							else if (*p == 'i')
								p = ReadMap(data, p + 1, mtl->map_Ni);
						}
					}
					else if (*p == 'd') {
						p++;
						if (IsWhitespace(*p))
							p = ReadMap(data, p, mtl->map_d);
					}
					else if (p[0] == 'b' &&
						p[1] == 'u' &&
						p[2] == 'm' &&
						p[3] == 'p' &&
						IsWhitespace(p[4]))
					{
						p = ReadMap(data, p + 4, mtl->map_bump);
					}
				}
				break;

			case '#':
				break;
			}

			p = SkipLine(p);
		}

		/* Push final material */
		if (!mtl->name.empty())
			data.mesh->materials.push_back(mtl);

		free(contents);

		return 1;
	}
}
