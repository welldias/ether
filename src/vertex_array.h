#ifndef __ETHER_VERTEX_ARRAY_H__
#define __ETHER_VERTEX_ARRAY_H__

namespace ether {
	class VertexArray {
	public:
		VertexArray();
		~VertexArray();

		void Bind() const;
		void UnBind() const;

		void AddIndex(IVertexBuffer* vertexBuffer);
		void Add(IVertexBuffer* vertexBuffer);
		void Load();
		void Draw() const;

		ConstProperty<unsigned int> Id;

	private:
		unsigned int id;
		IVertexBuffer* indexBuffer;
		std::vector<IVertexBuffer*> vertexBuffers;
	};
}

#endif // __ETHER_VERTEX_ARRAY_H__