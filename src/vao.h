#ifndef __ETHER_VAO_H__
#define __ETHER_VAO_H__

namespace ether {
	class Vao {
	public:
		Vao();
		~Vao();

		void Add(const Vbo& vbo);
		void Load();
		void Draw() const;

		inline unsigned int GetID() const { return id; }

	private:
		unsigned int id;
		Vbo indices;
		Vbo vertices;
		Vbo texture;
		Vbo normals;
		Vbo colours;
	};
}

#endif // __ETHER_VAO_H__


