#ifndef __ETHER_OBJ_FILE_H__
#define __ETHER_OBJ_FILE_H__

namespace ether {

	using ObjFileUnit = unsigned int;

	struct ObjFileIndex {
		ObjFileUnit p;
		ObjFileUnit t;
		ObjFileUnit n;
	};

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
		ObjFileTexture  mapKa;
		ObjFileTexture  mapKd;
		ObjFileTexture  mapKs;
		ObjFileTexture  mapKe;
		ObjFileTexture  mapKt;
		ObjFileTexture  mapNs;
		ObjFileTexture  mapNi;
		ObjFileTexture  mapD;
		ObjFileTexture  mapBump;

		ObjFileMaterial() {
			Ka[0] = Ka[1] = Ka[2] = 0.0;
			Kd[0] = Kd[1] = Kd[2] = 1.0;
			Ks[0] = Ks[1] = Ks[2] = 0.0;
			Ke[0] = Ke[1] = Ke[2] = 0.0;
			Kt[0] = Kt[1] = Kt[2] = 0.0;
			Ns = 1.0;
			Ni = 1.0;
			Tf[0] = Tf[1] = Tf[2] = 1.0;
			d = 1.0;
			illum = 1;
		}
	};

	struct ObjFileGroup {

		/* Group name */
		std::string name;

		ObjFileMaterial* material;

		/* Number of faces */
		unsigned int  faceCount;

		/* First face in fastObjMesh face_* arrays */
		unsigned int  faceIdx;

		ObjFileGroup() {
			material = NULL;
			faceCount = 0;
			faceIdx = 0;
		}
	};

	using ObjFileVertice = float[3];
	using ObjFileVeTextcoord = float[2];
	using ObjFileNormal = float[3];
	using ObjFileColor = float[3];
	//using ObjFileFace = ObjFileIndex[3];
	
	using ObjFileIndice = int[3];

	using ObjFileObjectList = std::vector<ObjFileGroup*>;
	using ObjFileMaterialList = std::vector<ObjFileMaterial*>;

	class ObjFile {
	public:
		ObjFile(const std::string& filePath);
		~ObjFile();
		void Load();

		unsigned int TotalVertex() const { return vertexCount;  }
		unsigned int TotalIndices() const { return indexCount;  }
		unsigned int TotalTextcoords() const { return vertexCount; }
		unsigned int TotalColors() const { return vertexCount; }
		unsigned int TotalNormals() const { return vertexCount; }

		ObjFileVertice* Vertices() { return vertices; }
		ObjFileIndice* Indices() { return indices; }
		ObjFileVeTextcoord* Textcoords() { return indexedTextcoord; }
		ObjFileColor* Colors() { return indexedColours; }
		ObjFileNormal* Normals() { return indexedNormal; }

	private:
		char* LoadFileContent();
		void PreProcess(const char* ptr, const char* end);
		void Process(const char* ptr, const char* end);

		const char* ParseVertex(const char* ptr);
		const char* ParseTexcoord(const char* ptr);
		const char* ParseNormal(const char* ptr);
		const char* ParseFace(const char* ptr);
		const char* ParseObject(const char* ptr);
		const char* ParseMtllib(const char* ptr);
		const char* ParseUsemtl(const char* ptr);
		const char* ParseFloat(const char* ptr, float* val);
		const char* ParseInt(const char* ptr, int* val);

		const char* ReadMtlTriple(const char* p, float v[3]);
		const char* ReadMap(const char* ptr, ObjFileTexture& map);
		void FlushObject(std::string& name);

		inline int IsWhitespace(char c) { return (c == ' ' || c == '\t' || c == '\r'); }
		inline int IsEndOfName(char c) { return (c == '\t' || c == '\r' || c == '\n' || c == '\0'); }
		inline int IsNewline(char c) { return (c == '\n' || c == '\0'); }
		inline int IsDigit(char c) { return (c >= '0' && c <= '9'); }
		inline int IsExponent(char c) { return (c == 'e' || c == 'E'); }
		inline const char* SkipWhitespace(const char* ptr) { while (IsWhitespace(*ptr)) ptr++; return ptr; }
		inline const char* SkipLine(const char* ptr) { while (!IsNewline(*ptr++)); return ptr; }
		inline const char* ReadMtlInt(const char* p, int* v) { return ParseInt(p, v); }
		inline const char* ReadMtlSingle(const char* p, float* v) { return ParseFloat(p, v); }

	private:
		unsigned int vertexCount;
		unsigned int vertexIdx;
		ObjFileVertice* vertices;
		ObjFileVeTextcoord* indexedTextcoord;
		ObjFileColor* indexedColours;
		ObjFileNormal* indexedNormal;

		unsigned int indexCount;
		unsigned int indexIdx;
		//ObjFileFace* faces;
		ObjFileIndice* indices;

		unsigned int textcoordCount;
		unsigned int textcoordIdx;
		ObjFileVeTextcoord* texcoords;

		unsigned int normalCount;
		unsigned int normalIdx;
		ObjFileNormal* normals;

		std::string filePath;
		std::string baseDir;

		unsigned int lineNumber;
		bool useTextureCoords;

		/* Materials */
		ObjFileMaterialList materials;

		/* Object list */
		ObjFileObjectList objects;

	private:
		static const char pathSeparator;
		static const char otherSep;
		static const double power10Pos[];
		static const double power10Neg[];
	};
}

#endif // __ETHER_OBJ_FILE_H__