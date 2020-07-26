#include "ether.h"

namespace ether {

	Vao::Vao() {
		attributeNumber = 0;
		glGenVertexArrays(1, &id);
	}

	Vao::~Vao() {
		glBindVertexArray(id);
		for (auto vbo : vbos) {
			vbo.ReleaseBuffer();
		}
		glDeleteVertexArrays(1, &id);
		glBindVertexArray(0);
	}

	void Vao::Add(const Vbo& vbo) {
		vbos.push_back(vbo);
	}

	void Vao::Load() {

		glBindVertexArray(id);

		for (auto it = vbos.begin(); it != vbos.end(); it++) {
			if ((*it).GetType() == Vbo::Type::Indices) {
				(*it).Load(attributeNumber);
				break;
			}
		}

		for (auto it = vbos.begin(); it != vbos.end(); it++) {
			if ((*it).GetType() != Vbo::Type::Indices) {
				(*it).Load(attributeNumber);
				glBindBuffer(GL_ARRAY_BUFFER, attributeNumber);
				attributeNumber++;
			}
		}

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
		unsigned int indicesCount = 0;
		for (auto it = vbos.begin(); it != vbos.end(); it++) {
			if ((*it).GetType() == Vbo::Type::Indices) {
				indicesCount = (*it).GetCount();
				break;
			}
		}
		
		if (indicesCount == 0)
			return;

		glBindVertexArray(id);
		glEnableVertexAttribArray(0);
		
		glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);

		glDisableVertexAttribArray(0);
		glBindVertexArray(0);

#endif
		
	}
}
