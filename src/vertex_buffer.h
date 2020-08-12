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

		virtual void ReleaseBuffer() = 0;
		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

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
		VertexBuffer(const void* data, unsigned int size, unsigned int attributeCount) {

			this->type = GetType<T>();
			this->size = size;
			this->count = size / sizeof(T);
			this->attributeCount = attributeCount;
			
			glGenBuffers(1, &id);
			Bind();
			glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		}

		~VertexBuffer() {
			ReleaseBuffer();
		}

		virtual void ReleaseBuffer() {
			glDeleteBuffers(1, &id);
			id = 0;
		}

		virtual void Bind() const {
			glBindBuffer(GL_ARRAY_BUFFER, id);
		}

		virtual void UnBind() const {
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		virtual void EnableAttribute(unsigned int attribute) {

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

			//Bind();
			glVertexAttribPointer(attribute, attributeCount, glType, glNormalized, attributeCount * sizeof(T), (void*)0);
			glEnableVertexAttribArray(attribute);
		}

	private:
		unsigned int attributeCount;
	};
}

#endif // __ETHER_VERTEX_BUFFER_H__