#ifndef __ETHER_VERTEX_BUFFER_H__
#define __ETHER_VERTEX_BUFFER_H__

namespace ether {

	class VertexBuffer
	{
	public:
		enum class Type {
			Undefined,
			Vertices,
			Indices,
			Colours,
			Texture,
			Normals,
		};

		VertexBuffer();
		VertexBuffer(VertexBuffer::Type type, unsigned int count, void* data, unsigned textureID=0);
		~VertexBuffer();

		void Load(unsigned int attributeNumber);
		void ReleaseBuffer();
		unsigned int Size() const;

		void EnableAttributeArray() const;
		void DisableAttributeArray() const;

		inline unsigned int GetId() const { return id; };
		inline const void* GetData() const { return data; };
		inline unsigned int GetCount() const { return count; };
		inline unsigned int GetVertexCount() const { return count/3; };
		inline Type GetType() const { return type; };
		inline unsigned int GetAttributeNumber() const { return attributeNumber; };

	private:
		void BindArrayByffer(unsigned int attributeNumber);
		void BindElementArrayByffer();

	private:
		unsigned int id;
		unsigned int attributeNumber;
		unsigned int textureID;

		void* data;
		unsigned int count;
		Type type;
	};
}

#endif // __ETHER_VERTEX_BUFFER_H__