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
		case Vbo::Type::Colours:
		default:
			throw EngineError("Databuffer type unsupported");
		}
	}

	void Vao::Load() {

		glBindVertexArray(id);

		indices.Load(0);
		vertices.Load(0);
		texture.Load(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Vao::Draw() const {

		if (id == 0)
			return;

#if 0
		unsigned int indicesCount = 0;
		for (auto it = vbos.begin(); it != vbos.end(); it++) {
			if ((*it).GetType() == Vbo::Type::Indices) {
				indicesCount = (*it).GetCount();
				break;
			}
		}

		glBindVertexArray(id);
		glEnableVertexAttribArray(0);

		for (auto it = vbos.begin(); it != vbos.end(); it++) {
			if ((*it).GetId() != 0) {
				glDrawArrays(GL_TRIANGLES, 0, (*it).GetVertexCount());
			}
		}
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
#else
		
		if (indices.GetCount() == 0)
			return;


		glBindVertexArray(id);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture.GetId());

		glDrawElements(GL_TRIANGLES, indices.GetCount(), GL_UNSIGNED_INT, 0);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glBindVertexArray(0);

#endif
		
	}
}
