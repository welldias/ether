#ifndef __ETHER_INDEX_BUFFER_H__
#define __ETHER_INDEX_BUFFER_H__

namespace ether {

	template <class T>
	class IndexBuffer : public  IVertexBuffer {
	public:
		IndexBuffer(const void* data, unsigned int count);
		~IndexBuffer();

		virtual void EnableAttribute(unsigned int attribute) { return; }
	};

	template <class T>
	IndexBuffer<T>::IndexBuffer(const void* data, unsigned int count) {

		//assert supported type
		GetType<T>();

		this->count = count;
		this->size = sizeof(T) * count;

		//if (typeid(T) == typeid(int)) {
		//	this->size = sizeof(int) * count;
		//}
		//else if (typeid(T) == typeid(unsigned int)) {
		//	this->size = sizeof(unsigned int) * count;
		//}
		//else if (typeid(T) == typeid(short)) {
		//	this->size = sizeof(short) * count;
		//}
		//else if (typeid(T) == typeid(unsigned short)) {
		//	this->size = sizeof(short) * count;
		//}
		//else if (typeid(T) == typeid(unsigned char)) {
		//	this->size = count;
		//}
		//else if (typeid(T) == typeid(char)) {
		//	this->size = count;
		//}
		//else {
		//	throw EngineError("Unsupported type");
		//}

		glGenBuffers(1, &id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->size, static_cast<const void*>(data), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	template <class T>
	IndexBuffer<T>::~IndexBuffer() {
		ReleaseBuffer();
	}
}

#endif // __ETHER_INDEX_BUFFER_H__