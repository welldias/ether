#include "ether.h"

namespace ether {

	unsigned int Vao::vertexIndiceCount = 0;

	Vao::Vao() {
		glGenVertexArrays(1, &id);
	}

	Vao::~Vao() {
		glBindVertexArray(id);

		indices.ReleaseBuffer();
		vertices.ReleaseBuffer();
		texture.ReleaseBuffer();

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
		vertices.Load(NextIdx());
		texture.Load(NextIdx());
		normals.Load(NextIdx());
		colours.Load(NextIdx());

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Vao::Draw() const {

		if (id == 0)
			return;
	
		if (indices.GetCount() == 0)
			return;

		glBindVertexArray(id);

		vertices.EnableAttributeArray();
		texture.EnableAttributeArray();

		glDrawElements(GL_TRIANGLES, (GLsizei)indices.GetCount(), GL_UNSIGNED_INT, 0);

		texture.DisableAttributeArray();
		vertices.DisableAttributeArray();

		glBindVertexArray(0);
	}
}
