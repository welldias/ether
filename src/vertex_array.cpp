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
		glBindBuffer(GL_ARRAY_BUFFER, this->id);
	}

	void VertexArray::UnBind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexArray::AddIndex(IVertexBuffer* indexBuffer) {
		this->indexBuffer = indexBuffer;
	}

	void VertexArray::Add(IVertexBuffer* vertexBuffer) {
		vertexBuffers.push_back(vertexBuffer);
	}

	void VertexArray::Load() {

		Bind();

		unsigned int attribute = 0;
		for (auto vb : vertexBuffers) {
			vb->EnableAttribute(attribute);
			attribute++;
		}

		//UnBind();
	}

	void VertexArray::Draw() const {

		//if (indices.GetCount() == 0)
		//	return;

		Bind();
		indexBuffer->Bind();

		//vertices.EnableAttributeArray();
		//normals.EnableAttributeArray();
		//texture.EnableAttributeArray();
		//colours.EnableAttributeArray();

		//glDrawElements(GL_TRIANGLES, (GLsizei)indices.GetCount(), GL_UNSIGNED_INT, 0);

		//colours.DisableAttributeArray();
		//texture.DisableAttributeArray();
		//normals.DisableAttributeArray();
		//vertices.DisableAttributeArray();

		//UnBind();
	}
}
