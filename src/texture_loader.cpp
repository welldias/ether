#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "ether.h"

namespace ether {

	TextureLoader::TextureLoader(const std::string& filePath) {
		this->id = 0;
		this->width = 0;
		this->height = 0;
		this->channels = 0;
		this->path = filePath;
	}

	TextureLoader::~TextureLoader() {
	}

	void TextureLoader::Load()
	{
		void* content = stbi_load(path.c_str(), &width, &height, &channels, 0);
		if (content == NULL) {
			throw EngineError("Texture file ("+ path +") not found");
		}

		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, content);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(content);
	}

}
