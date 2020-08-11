#ifndef __ETHER_VERTEX_BUFFER_H__
#define __ETHER_VERTEX_BUFFER_H__

namespace ether {

	class IVertexBuffer
	{
	public:
		enum class BufferType {
			Complex,
			Float,
			Int,
			UnsignedInt,
			Short,
			UnsignedShort,
			Byte,
			UnsignedByte,
		};

		IVertexBuffer() : Type(&type), Id(&id), Count(&count), Size(&size) {}

		virtual void EnableAttribute(unsigned int attribute) = 0;

		template <typename T>
		BufferType GetType() {
		
			if (typeid(T) == typeid(float))
				return IVertexBuffer::BufferType::Float;
			if (typeid(T) == typeid(int))
				return IVertexBuffer::BufferType::Int;
			if (typeid(T) == typeid(unsigned int))
				return IVertexBuffer::BufferType::UnsignedInt;
			if (typeid(T) == typeid(short))
				return IVertexBuffer::BufferType::Short;
			if (typeid(T) == typeid(unsigned short))
				return IVertexBuffer::BufferType::UnsignedShort;
			if (typeid(T) == typeid(char))
				return IVertexBuffer::BufferType::UnsignedByte;
			if (typeid(T) == typeid(unsigned char))
				return IVertexBuffer::BufferType::UnsignedByte;
		
			throw EngineError("Unsupported type");
		}


		ConstProperty<BufferType> Type;
		ConstProperty<unsigned int> Id;
		ConstProperty<unsigned int> Count;
		ConstProperty<unsigned int> Size;

		void ReleaseBuffer();
		void Bind() const;
		void UnBind() const;

	protected:
		BufferType type;
		unsigned int id;
		unsigned int count;
		unsigned int size;
	};

	template <typename T>
	class VertexBuffer : public  IVertexBuffer
	{
	public:
		VertexBuffer(const void* data, unsigned int count, unsigned int attributeCount);
		~VertexBuffer();

		virtual void EnableAttribute(unsigned int attribute);
	private:
		unsigned int attributeCount;
	};

	template <typename T>
	VertexBuffer<T>::VertexBuffer(const void* data, unsigned int count, unsigned int attributeCount) {

		this->type = GetType<T>();
		this->count = count * attributeCount;
		this->attributeCount = attributeCount;
		this->size = sizeof(T) * this->count;

		//if (typeid(T) == typeid(float)) {
		//	this->size = sizeof(float) * count;
		//	type = BufferType::Float;
		//}
		//else if (typeid(T) == typeid(int)) {
		//	this->size = sizeof(int) * count;
		//	type = BufferType::Int;
		//}
		//else if (typeid(T) == typeid(unsigned int)) {
		//	this->size = sizeof(int) * count;
		//	type = BufferType::UnsignedInt;
		//}
		//else if (typeid(T) == typeid(short)) {
		//	this->size = sizeof(short) * count;
		//	type = BufferType::Short;
		//}
		//else if (typeid(T) == typeid(unsigned short)) {
		//	this->size = sizeof(short) * count;
		//	type = BufferType::UnsignedShort;
		//}
		//else if (typeid(T) == typeid(char)) {
		//	this->size = count;
		//	type = BufferType::UnsignedByte;
		//}
		//else if (typeid(T) == typeid(unsigned char)) {
		//	this->size = count;
		//	type = BufferType::UnsignedByte;
		//}
		//else {
		//	throw EngineError("Unsupported type");
		//}

		glGenBuffers(1, &(this->id));
		glBindBuffer(GL_ARRAY_BUFFER, this->id);
		glBufferData(GL_ARRAY_BUFFER, this->size, static_cast<const void*>(data), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	template <typename T>
	VertexBuffer<T>::~VertexBuffer() {
		ReleaseBuffer();
	}

	template<typename T>
	void VertexBuffer<T>::EnableAttribute(unsigned int id) {

		GLenum glType = 0;
		GLboolean glNormalized = GL_FALSE;

		switch (type)
		{
		case BufferType::Float:
			glType = GL_FLOAT;
			break;
		case BufferType::Int:
			glType = GL_INT;
			break;
		case BufferType::UnsignedInt:
			glType = GL_UNSIGNED_INT;
			break;
		case BufferType::Short:
			glType = GL_SHORT;
			break;
		case BufferType::UnsignedShort:
			glType = GL_UNSIGNED_SHORT;
			break;
		case BufferType::Byte:
			glType = GL_BYTE;
			glNormalized = GL_TRUE;
			break;
		case BufferType::UnsignedByte:
			glType = GL_UNSIGNED_BYTE;
			glNormalized = GL_TRUE;
			break;
		default:
			throw EngineError("Unsupported type");
		}

		Bind();
		glEnableVertexAttribArray(id);
		glVertexAttribPointer(id, attributeCount, glType, glNormalized, 0, 0);
	}
}

#endif // __ETHER_VERTEX_BUFFER_H__