#include "ether.h"

namespace ether {

	Vao::Vao() {
		glGenVertexArrays(1, &id);
	}

	Vao::~Vao() {
		glBindVertexArray(id);

		indices.ReleaseBuffer();
		vertices.ReleaseBuffer();
		texture.ReleaseBuffer();
		normals.ReleaseBuffer();
		colours.ReleaseBuffer();

		glDeleteVertexArrays(1, &id);
		glBindVertexArray(0);
	}

	void Vao::Add(const Vbo& vbo) {
		
		switch (vbo.GetType())
		{
		case Vbo::Type::Indices:
			this->indices = vbo;
			break;
		case Vbo::Type::Vertices:
			this->vertices = vbo;
			break;
		case Vbo::Type::Texture:
			this->texture = vbo;
			break;
		case Vbo::Type::Normals:
			this->normals = vbo;
			break;
		case Vbo::Type::Colours:
			this->colours = vbo;
			break;
		default:
			throw EngineError("Databuffer type unsupported");
		}
	}

	void Vao::Load() {

		glBindVertexArray(id);

		indices.Load(0);
		vertices.Load(0);
		//texture.Load(1);
		colours.Load(1);
		normals.Load(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Vao::Draw() const {

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
