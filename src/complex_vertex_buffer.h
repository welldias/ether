#ifndef __ETHER_COMPLEX_VERTEX_BUFFER_H__
#define __ETHER_COMPLEX_VERTEX_BUFFER_H__

namespace ether {

	class ComplexVertexBuffer : public  IVertexBuffer
	{
	public:
		struct Layout {

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

			template <typename T>
			BufferType GetType() {

				if (typeid(T) == typeid(float))
					return BufferType::Float;
				if (typeid(T) == typeid(int))
					return BufferType::Int;
				if (typeid(T) == typeid(unsigned int))
					return BufferType::UnsignedInt;
				if (typeid(T) == typeid(short))
					return BufferType::Short;
				if (typeid(T) == typeid(unsigned short))
					return BufferType::UnsignedShort;
				if (typeid(T) == typeid(char))
					return BufferType::UnsignedByte;
				if (typeid(T) == typeid(unsigned char))
					return BufferType::UnsignedByte;

				throw EngineError("Unsupported type");
			}

			struct Element {
				unsigned int count;
				BufferType type;
				bool normalized;
			};

			template <typename T>
			void Push(unsigned int count) {

				BufferType type = GetType<T>();
				bool normalized = (typeid(T) == typeid(char) || typeid(T) == typeid(unsigned char)) ? true :  false;
				Elements.push_back({ count, type, normalized });
			}

			std::vector<Element> Elements;
		};

		ComplexVertexBuffer(const void* data, const Layout& layout);
		~ComplexVertexBuffer();

		virtual void ReleaseBuffer() {
			//glDeleteBuffers(1, &id);
			//id = 0;
		}

		virtual void Bind() const {
			//glBindBuffer(GL_ARRAY_BUFFER, this->id);
		}

		virtual void UnBind() const {
		//	glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		virtual void EnableAttribute(unsigned int attribute);

	private:
		Layout layout;
	};
}

#endif // __ETHER_COMPLEX_VERTEX_BUFFER_H__