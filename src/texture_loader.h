#ifndef __ETHER_TEXTURE_LOADER_H__
#define __ETHER_TEXTURE_LOADER_H__

namespace ether {

	class TextureLoader
	{
	public:
		TextureLoader(const std::string& filePath);
		~TextureLoader();

		void Load();

		inline int GetId() const { return id;  }
		inline int GetWidth() const { return width; }
		inline int GetHeight() const { return height; }

	private:
		unsigned int id;
		std::string path;
		int width;
		int height;
		int channels;
	};
}

#endif // __ETHER_TEXTURE_LOADER_H__