#ifndef __ETHER_MESH_H__
#define __ETHER_MESH_H__

namespace ether {

	class Mesh
	{
	public:
		Mesh();
		~Mesh();

		unsigned int VerticesSize();
		unsigned int IndicesSize();
		unsigned int Size();

		float* vertices;
		unsigned int* indices;

		unsigned int totalVertices;
		unsigned int totalIndices;
	};
}

#endif // __ETHER_MESH_H__