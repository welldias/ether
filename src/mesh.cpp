#include "ether.h"

namespace ether {

	Mesh::Mesh() {
		totalVertices = 0;
		totalIndices = 0;
		vertices = NULL;
		indices = NULL;
	}

	Mesh::~Mesh() {

		free(vertices);
		free(indices);
	}

	unsigned int Mesh::VerticesSize() {
		return (unsigned int)(sizeof(float) * totalVertices * 3);
	}

	unsigned int Mesh::IndicesSize() {
		return (unsigned int)(sizeof(int) * totalIndices);
	}

	unsigned int Mesh::Size() {
		return IndicesSize() + VerticesSize();
	}

}
