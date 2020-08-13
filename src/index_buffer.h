#ifndef __ETHER_INDEX_BUFFER_H__
#define __ETHER_INDEX_BUFFER_H__

namespace ether {

	template <class T>
	class IndexBuffer : public  IVertexBuffer {
	public:
		IndexBuffer(const void* data, unsigned int size) {

			this->count = size / sizeof(T);
			this->size = size;

			glGenBuffers(1, &id);
			Bind();
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		}

		~IndexBuffer() {
			ReleaseBuffer();
		}

		virtual void ReleaseBuffer() {
			glDeleteBuffers(1, &id);
			id = 0;
		}

		virtual void Bind() const {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
		}

		virtual void UnBind() const {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		virtual void EnableAttribute(unsigned int attribute) { return; }
	};
}

#endif // __ETHER_INDEX_BUFFER_H__