#ifndef __ETHER_VBO_H__
#define __ETHER_VBO_H__

namespace ether {

	class Vbo
	{
	public:
		enum class Type {
			Vertices = 0,
			Indices,
			Colours,
			Texture,
			Normals,
		};

		Vbo(Vbo::Type type, unsigned int count, void* data);
		~Vbo();

		void Load(unsigned int attributeNumber);
		void AllocData();
		void ReleaseBuffer();
		unsigned int UnitSize() const;
		unsigned int Size() const;
		unsigned int GlType() const;

		unsigned int GetId() const { return id; };
		const void* GetData() const { return data; };
		unsigned int GetCount() const { return count; };
		unsigned int GetVertexCount() const { return count/3; };
		Type GetType() const { return type; };
	
	private:
		void BindArrayByffer(unsigned int attributeNumber);
		void BindElementArrayByffer();

	private:
		unsigned int id;

		void* data;
		unsigned int count;
		Type type;
	};
}

#endif // __ETHER_VBO_H__


