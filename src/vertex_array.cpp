#include "ether.h"

namespace ether {

	VertexArray::VertexArray() {
		glGenVertexArrays(1, &id);
	}

	VertexArray::~VertexArray() {
		glBindVertexArray(id);

		indices.ReleaseBuffer();
		vertices.ReleaseBuffer();
		texture.ReleaseBuffer();
		normals.ReleaseBuffer();
		colours.ReleaseBuffer();

		glDeleteVertexArrays(1, &id);
		glBindVertexArray(0);
	}

	void VertexArray::Add(const VertexBuffer& vbo) {
		
		switch (vbo.GetType())
		{
		case VertexBuffer::Type::Indices:
			this->indices = vbo;
			break;
		case VertexBuffer::Type::Vertices:
			this->vertices = vbo;
			break;
		case VertexBuffer::Type::Texture:
			this->texture = vbo;
			break;
		case VertexBuffer::Type::Normals:
			this->normals = vbo;
			break;
		case VertexBuffer::Type::Colours:
			this->colours = vbo;
			break;
		default:
			throw EngineError("Databuffer type unsupported");
		}
	}

	void VertexArray::Load() {

		glBindVertexArray(id);

		indices.Load(0);
		vertices.Load(0);
		//texture.Load(1);
		colours.Load(1);
		normals.Load(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexArray::Draw() const {

		if (id == 0)
			return;
	
		if (indices.GetCount() == 0)
			return;

		glBindVertexArray(id);

		vertices.EnableAttributeArray();
		normals.EnableAttributeArray();
		texture.EnableAttributeArray();
		colours.EnableAttributeArray();

		glDrawElements(GL_TRIANGLES, (GLsizei)indices.GetCount(), GL_UNSIGNED_INT, 0);

		colours.DisableAttributeArray();
		texture.DisableAttributeArray();
		normals.DisableAttributeArray();
		vertices.DisableAttributeArray();

		glBindVertexArray(0);
	}
}
