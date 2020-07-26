#include "ether.h"

namespace ether {

	Vbo::Vbo(Vbo::Type type, unsigned int count, void* data) {

		this->id = 0;
		this->type = type;
		this->count = count;
		this->data = data;
	}

	Vbo::~Vbo() {

		if(id != 0)
			glDeleteBuffers(1, &id);

		//operator delete(data);
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
			BindArrayByffer(attributeNumber);
			break;
		case Vbo::Type::Indices:
			BindElementArrayByffer();
			break;
		case Vbo::Type::Normals:
		case Vbo::Type::Colours:
		case Vbo::Type::Texture:
			break;
		}
	}

	void Vbo::AllocData() {
		if (this->data != NULL) {
			throw EngineError("Data of DataBuffer already allocated");
		}

		data = operator new(Size());
	}

	unsigned int Vbo::UnitSize() const {
		switch (this->type)
		{
		case Vbo::Type::Vertices:
		case Vbo::Type::Normals:
		case Vbo::Type::Colours:
			return sizeof(float) * 3;
		case Vbo::Type::Indices:
			return sizeof(int) * 3;
		case Vbo::Type::Texture:
			return sizeof(unsigned char) * 2;
		}

		throw EngineError("Databuffer type undefined");
	}

	unsigned int Vbo::Size() const {
		switch (this->type)
		{
		case Vbo::Type::Vertices:
		case Vbo::Type::Normals:
		case Vbo::Type::Colours:
			return sizeof(float) * count;
		case Vbo::Type::Indices:
			return sizeof(int) * count;
		case Vbo::Type::Texture:
			return sizeof(unsigned char) * count;
		}

		throw EngineError("Databuffer type undefined");
	}

	unsigned int Vbo::GlType() const {
		switch (this->type)
		{
		case Vbo::Type::Vertices:
		case Vbo::Type::Normals:
		case Vbo::Type::Colours:
			return GL_FLOAT;
		case Vbo::Type::Indices:
			return  GL_UNSIGNED_INT;
		case Vbo::Type::Texture:
			return GL_UNSIGNED_BYTE;
		}

		throw EngineError("Databuffer type undefined");
	}

	void Vbo::BindArrayByffer(unsigned int attributeNumber) {
		glGenBuffers(1, &id);
		glBindBuffer(GL_ARRAY_BUFFER, id);
		glBufferData(GL_ARRAY_BUFFER, Size(), data, GL_STATIC_DRAW);
		glVertexAttribPointer(attributeNumber, 3, GlType(), GL_FALSE, UnitSize(), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Vbo::BindElementArrayByffer() {
		glGenBuffers(1, &id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, Size(), data, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}
