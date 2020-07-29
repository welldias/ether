#ifndef __ETHER_OBJ_FILE_H__
#define __ETHER_OBJ_FILE_H__

namespace ether {

	using ObjFileUnit = unsigned int;

	struct ObjFileTexture {
		/* Texture name from .mtl file */
		std::string name;

		/* Resolved path to texture */
		std::string path;
	};

	struct ObjFileMaterial {
		/* Material name */
		std::string name;

		/* Parameters */
		float  Ka[3];  /* Ambient */
		float  Kd[3];  /* Diffuse */
		float  Ks[3];  /* Specular */
		float  Ke[3];  /* Emission */
		float  Kt[3];  /* Transmittance */
		float  Ns;     /* Shininess */
		float  Ni;     /* Index of refraction */
		float  Tf[3];  /* Transmission filter */
		float  d;      /* Disolve (alpha) */
		int    illum;  /* Illumination model */

		/* Texture maps */
		ObjFileTexture  map_Ka;
		ObjFileTexture  map_Kd;
		ObjFileTexture  map_Ks;
		ObjFileTexture  map_Ke;
		ObjFileTexture  map_Kt;
		ObjFileTexture  map_Ns;
		ObjFileTexture  map_Ni;
		ObjFileTexture  map_d;
		ObjFileTexture  map_bump;
	};

	struct ObjFileIndex {
		ObjFileUnit p;
		ObjFileUnit t;
		ObjFileUnit n;
	};


	struct ObjFileGroup {

		/* Group name */
		std::string name;

		/* Number of faces */
		unsigned int  face_count;

		/* First face in fastObjMesh face_* arrays */
		unsigned int  face_offset;

		/* First index in fastObjMesh indices array */
		unsigned int  index_offset;
	};

	using ObjFileGroupList = std::vector<ObjFileGroup*>;
	using ObjFileMaterialList = std::vector<ObjFileMaterial*>;

	class ObjFile {
	public:
		ObjFile(const std::string& filePath);
		~ObjFile();
		void Load();

	private:
		struct ObjFileData
		{
			/* Final mesh */
			ObjFile* mesh;

			/* Current group */
			ObjFileGroup group;

			/* Current material index */
			unsigned int material;

			/* Current line in file */
			unsigned int line;

			/* Base path for materials/textures */
			char* base;
		};

	private:

		std::string filePath;

		/* Vertex data */
		unsigned int  position_count;
		float* positions;

		unsigned int texcoord_count;
		float* texcoords;

		unsigned int normal_count;
		float* normals;

		/* Face data: one element for each face */
		unsigned int  face_count;
		unsigned int* face_vertices;
		unsigned int* face_materials;

		/* Index data: one element for each face vertex */
		ObjFileIndex* indices;

		/* Materials */
		ObjFileMaterialList materials;

		/* Mesh groups */
		ObjFileGroupList groups;

	private:
		void ParseBuffer(ObjFileData& data, const char* ptr, const char* end);

		void FlushOutput(ObjFileData& data);
		void GroupDefault(ObjFileGroup& group);

		const char* ParseVertex(ObjFileData& data, const char* ptr);
		const char* ParseTexcoord(ObjFileData& data, const char* ptr);
		const char* ParseNormal(ObjFileData& data, const char* ptr);
		const char* ParseFace(ObjFileData& data, const char* ptr);
		const char* ParseGroup(ObjFileData& data, const char* ptr);
		const char* ParseMtllib(ObjFileData& data, const char* ptr);
		const char* ParseUsemtl(ObjFileData& data, const char* ptr);
		const char* ParseFloat(const char* ptr, float* val);
		const char* ParseInt(const char* ptr, int* val);

		int ReadMtllib(ObjFileData& data, FILE* file);
		const char* ReadMtlTriple(const char* p, float v[3]);
		const char* ReadMap(ObjFileData& data, const char* ptr, ObjFileTexture& map);

		ObjFileMaterial* MtlDefault();

		void* ArrayRealloc(void* ptr, ObjFileUnit n, ObjFileUnit b);
		
		unsigned long FileSize(void* file);
		char* StringCopy(const char* s, const char* e);
		char* StringSubstr(const char* s, size_t a, size_t b);
		char* StringConcat(const char* a, const char* s, const char* e);
		int StringEqual(const char* a, const char* s, const char* e);
		int StringFindLast(const char* s, char c, size_t* p);
		void StringFixSeparators(char* s);

		inline int IsWhitespace(char c) { return (c == ' ' || c == '\t' || c == '\r'); }
		inline int IsEndOfName(char c) { return (c == '\t' || c == '\r' || c == '\n'); }
		inline int IsNewline(char c) { return (c == '\n'); }
		inline int IsDigit(char c) { return (c >= '0' && c <= '9'); }
		inline int IsExponent(char c) { return (c == 'e' || c == 'E'); }
		inline const char* SkipWhitespace(const char* ptr) { while (IsWhitespace(*ptr)) ptr++; return ptr; }
		inline const char* SkipLine(const char* ptr) { while (!IsNewline(*ptr++)); return ptr; }
		inline const char* ReadMtlInt(const char* p, int* v) { return ParseInt(p, v); }
		inline const char* ReadMtlSingle(const char* p, float* v) { return ParseFloat(p, v); }

	private:
		static const char pathSeparator;
		static const char otherSep;
		static const double power10Pos[];
		static const double power10Neg[];
	};
}

#endif // __ETHER_OBJ_FILE_H__