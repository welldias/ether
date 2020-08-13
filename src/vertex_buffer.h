#ifndef __ETHER_VERTEX_BUFFER_H__
#define __ETHER_VERTEX_BUFFER_H__

namespace ether {

	class IVertexBuffer
	{
	public:

		IVertexBuffer() : Id(&id), Count(&count), Size(&size) {}

		virtual void EnableAttribute(unsigned int attribute) = 0;

		ConstProperty<unsigned int> Id;
		ConstProperty<unsigned int> Count;
		ConstProperty<unsigned int> Size;

		virtual void ReleaseBuffer() = 0;
		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

	protected:
		unsigned int id;
		unsigned int count;
		unsigned int size;
	};

	template <typename T>
	class VertexBuffer : public  IVertexBuffer
	{
	public:
		VertexBuffer(const void* data, unsigned int size, unsigned int attributeCount) {

			if (typeid(T) != typeid(float) 
				&& typeid(T) != typeid(int) 
				&& typeid(T) != typeid(unsigned int)
				&& typeid(T) != typeid(short)
				&& typeid(T) != typeid(unsigned short) 
				&& typeid(T) != typeid(char) 
				&& typeid(T) != typeid(unsigned char)) {
				throw EngineError("Unsupported type");
			}

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

			if (typeid(T) == typeid(float))
				glType = GL_FLOAT;
			else if (typeid(T) == typeid(int))
				glType = GL_INT;
			else if (typeid(T) == typeid(unsigned int))
				glType = GL_UNSIGNED_INT;
			else if (typeid(T) == typeid(short))
				glType = GL_SHORT;
			else if (typeid(T) == typeid(unsigned short))
				glType = GL_UNSIGNED_SHORT;
			else if (typeid(T) == typeid(char)) {
				glType = GL_BYTE;
				glNormalized = GL_TRUE;
			}
			else if (typeid(T) == typeid(unsigned char)) {
				glType = GL_UNSIGNED_BYTE;
				glNormalized = GL_TRUE;
			}

			Bind();
			glVertexAttribPointer(attribute, attributeCount, glType, glNormalized, 0, 0);
			glEnableVertexAttribArray(attribute);
			//glVertexAttribPointer(attribute, attributeCount, glType, glNormalized, attributeCount * sizeof(T), (void*)0);
			UnBind();
		}

	private:
		unsigned int attributeCount;
	};
}

#endif // __ETHER_VERTEX_BUFFER_H__