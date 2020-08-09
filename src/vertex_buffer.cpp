#include "ether.h"

namespace ether {

	VertexBuffer::VertexBuffer() {

		this->id = 0;
		this->attributeNumber = 0;
		this->type = VertexBuffer::Type::Undefined;
		this->count = 0;
		this->data = NULL;
		this->textureID = 0;
	}


	VertexBuffer::VertexBuffer(VertexBuffer::Type type, unsigned int count, void* data, unsigned textureID) {

		this->id = 0;
		this->type = type;
		this->count = count;
		this->data = data;
		this->textureID = textureID;
	}

	VertexBuffer::~VertexBuffer() {
		ReleaseBuffer();
	}

	void VertexBuffer::ReleaseBuffer() {
		if (id == 0)
			return;

		glDeleteBuffers(1, &id);
		id = 0;
	}

	void  VertexBuffer::Load(unsigned int attributeNumber) {

		if (data == NULL || id != 0)
			return;

		switch (this->type)
		{
		case VertexBuffer::Type::Vertices:
		case VertexBuffer::Type::Texture:
		case VertexBuffer::Type::Colours:
		case VertexBuffer::Type::Normals:
			BindArrayByffer(attributeNumber);
			break;
		case VertexBuffer::Type::Indices:
			BindElementArrayByffer();
			break;
		default:
			throw EngineError("Databuffer type unsupported");
			break;
		}
	}

	unsigned int VertexBuffer::Size() const {
		switch (this->type)
		{
		case VertexBuffer::Type::Vertices:
		case VertexBuffer::Type::Texture:
		case VertexBuffer::Type::Normals:
		case VertexBuffer::Type::Colours:
			return sizeof(float) * count;
		case VertexBuffer::Type::Indices:
			return sizeof(int) * count;
		default:
			break;
		}

		throw EngineError("Databuffer type unsupported");
	}

	void VertexBuffer::BindArrayByffer(unsigned int attributeNumber) {

		unsigned int glUnitSize = 0;
		unsigned int glType = 0;
		switch (this->type)
		{
		case VertexBuffer::Type::Vertices:
		case VertexBuffer::Type::Normals:
			glUnitSize = 3;
			glType = GL_FLOAT;
			break;
		case VertexBuffer::Type::Colours:
			glUnitSize = 3;
			glType = GL_FLOAT;
			break;
		case VertexBuffer::Type::Texture:
			glUnitSize = 2;
			glType = GL_FLOAT;
			break;
		case VertexBuffer::Type::Indices:
		default:
			throw EngineError("Databuffer type unsupported");
		}

		this->attributeNumber = attributeNumber;

		glGenBuffers(1, &id);
		glBindBuffer(GL_ARRAY_BUFFER, id);
		glBufferData(GL_ARRAY_BUFFER, Size(), data, GL_STATIC_DRAW);
		glVertexAttribPointer(attributeNumber, glUnitSize, glType, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::BindElementArrayByffer() {
		glGenBuffers(1, &id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, Size(), data, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::EnableAttributeArray() const {
		if (id == 0)
			return;
		if (this->type == VertexBuffer::Type::Indices)
			return;
		glEnableVertexAttribArray(attributeNumber);

		if (this->type == VertexBuffer::Type::Texture) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, id);
		}
	}

	void VertexBuffer::DisableAttributeArray() const {
		if (id == 0)
			return;
		if (this->type == VertexBuffer::Type::Indices)
			return;
		glDisableVertexAttribArray(attributeNumber);
	}
}
