#ifndef __ETHER_VERTEX_ARRAY_H__
#define __ETHER_VERTEX_ARRAY_H__

namespace ether {
	class VertexArray {
	public:
		VertexArray();
		~VertexArray();

		void Add(const VertexBuffer& vbo);
		void Load();
		void Draw() const;

		inline unsigned int GetID() const { return id; }

	private:
		unsigned int id;
		VertexBuffer indices;
		VertexBuffer vertices;
		VertexBuffer texture;
		VertexBuffer normals;
		VertexBuffer colours;
	};
}

#endif // __ETHER_VERTEX_ARRAY_H__