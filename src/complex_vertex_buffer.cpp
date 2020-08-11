#include "ether.h"

namespace ether {

	ComplexVertexBuffer::ComplexVertexBuffer(const void* data, const Layout& layout) {

		this->layout = layout;

		glGenBuffers(1, &(this->id));
		glBindBuffer(GL_ARRAY_BUFFER, this->id);
		glBufferData(GL_ARRAY_BUFFER, this->size, static_cast<const void*>(data), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	ComplexVertexBuffer::~ComplexVertexBuffer() {
		ReleaseBuffer();
	}

	void ComplexVertexBuffer::EnableAttribute(unsigned int id) {
		throw EngineError("Complex Vertex Buffer error");
	}
}
