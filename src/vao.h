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
		unsigned int attributeNumber;
		std::vector<Vbo> vbos;
	};
}

#endif // __ETHER_VAO_H__


