#ifndef __ETHER_COMPLEX_VERTEX_BUFFER_H__
#define __ETHER_COMPLEX_VERTEX_BUFFER_H__

namespace ether {

	class ComplexVertexBuffer : public  IVertexBuffer
	{
	public:
		struct Layout {
			struct Element {
				unsigned int count;
				IVertexBuffer::BufferType type;
				bool normalized;
			};

			template <typename T>
			void Push(unsigned int count) {

				IVertexBuffer::BufferType type = IVertexBuffer::BufferType::Float;
				bool normalized = false;

				if (typeid(T) == typeid(float))
					type = IVertexBuffer::BufferType::Float;
				else if (typeid(T) == typeid(int))
					type = IVertexBuffer::BufferType::Int;
				else if (typeid(T) == typeid(unsigned int))
					type = IVertexBuffer::BufferType::UnsignedInt;
				else if (typeid(T) == typeid(short))
					type = IVertexBuffer::BufferType::Short;
				else if (typeid(T) == typeid(unsigned short))
					type = IVertexBuffer::BufferType::UnsignedShort;
				else if (typeid(T) == typeid(char)) {
					normalized = true;
					type = IVertexBuffer::BufferType::UnsignedByte;
				}
				else if (typeid(T) == typeid(unsigned char)) {
					normalized = true;
					type = IVertexBuffer::BufferType::UnsignedByte;
				}
				else
					throw EngineError("Unsupported type");

				Elements.push_back({ count, type, normalized });
			}

			std::vector<Element> Elements;
		};

		ComplexVertexBuffer(const void* data, const Layout& layout);
		~ComplexVertexBuffer();

		virtual void EnableAttribute(unsigned int attribute);

	private:
		Layout layout;
	};
}

#endif // __ETHER_COMPLEX_VERTEX_BUFFER_H__