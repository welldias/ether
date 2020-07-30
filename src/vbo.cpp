#include "ether.h"

namespace ether {

	Vbo::Vbo() {

		this->id = 0;
		this->type = Vbo::Type::Vertices;
		this->count = 0;
		this->data = NULL;
		this->textureID = 0;
	}


	Vbo::Vbo(Vbo::Type type, unsigned int count, void* data, unsigned textureID) {

		this->id = 0;
		this->type = type;
		this->count = count;
		this->data = data;
		this->textureID = textureID;
	}

	Vbo::~Vbo() {
		ReleaseBuffer();
	}

	void Vbo::ReleaseBuffer() {
		if (id == 0)
			return;

		glDeleteBuffers(1, &id);
		id = 0;
	}

	void  Vbo::Load(unsigned int attributeNumber) {

		if (data == NULL || id != 0)
			return;

		switch (this->type)
		{
		case Vbo::Type::Vertices:
		case Vbo::Type::Texture:
			BindArrayByffer(attributeNumber);
			break;
		case Vbo::Type::Indices:
			BindElementArrayByffer();
			break;
		case Vbo::Type::Normals:
		case Vbo::Type::Colours:
			throw EngineError("Databuffer type unsupported");
			break;
		}
	}

	unsigned int Vbo::Size() const {
		switch (this->type)
		{
		case Vbo::Type::Vertices:
		case Vbo::Type::Texture:
		case Vbo::Type::Normals:
			return sizeof(float) * count;
		case Vbo::Type::Indices:
			return sizeof(int) * count;
		case Vbo::Type::Colours:
		default:
			break;
		}

		throw EngineError("Databuffer type unsupported");
	}

	void Vbo::BindArrayByffer(unsigned int attributeNumber) {

		unsigned int glUnitSize = 0;
		unsigned int glType = 0;
		switch (this->type)
		{
		case Vbo::Type::Vertices:
		case Vbo::Type::Normals:
			glUnitSize = 3;
			glType = GL_FLOAT;
			break;
		case Vbo::Type::Texture:
			glUnitSize = 2;
			glType = GL_FLOAT;
			break;
		case Vbo::Type::Colours:
		case Vbo::Type::Indices:
		default:
			throw EngineError("Databuffer type unsupported");
		}

		glGenBuffers(1, &id);
		glBindBuffer(GL_ARRAY_BUFFER, id);
		glBufferData(GL_ARRAY_BUFFER, Size(), data, GL_STATIC_DRAW);
		glVertexAttribPointer(attributeNumber, glUnitSize, glType, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Vbo::BindElementArrayByffer() {
		glGenBuffers(1, &id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, Size(), data, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}
