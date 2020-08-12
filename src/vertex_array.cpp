#include "ether.h"

namespace ether {

	VertexArray::VertexArray()
		:Id(&id) {
		indexBuffer = NULL;
		glGenVertexArrays(1, &id);
	}

	VertexArray::~VertexArray() {
		Bind();

		for (auto vb : vertexBuffers) {
			delete vb;
		}
		vertexBuffers.clear();

		glDeleteVertexArrays(1, &id);
		UnBind();
	}

	void VertexArray::Bind() const {
		glBindVertexArray(id);
	}

	void VertexArray::UnBind() const {
		glBindVertexArray(0);
	}

	void VertexArray::AddIndex(IVertexBuffer* indexBuffer) {
		this->indexBuffer = indexBuffer;
	}

	void VertexArray::Add(IVertexBuffer* vertexBuffer) {
		vertexBuffers.push_back(vertexBuffer);
	}

	void VertexArray::Load() {

		unsigned int attribute = 0;
		for (auto vb : vertexBuffers) {
			vb->EnableAttribute(attribute);
			attribute++;
		}
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexArray::Draw() const {
		Bind();
		glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indexBuffer->Count), GL_UNSIGNED_INT, 0);
	}
}
