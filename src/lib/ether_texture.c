#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad/glad.h> 

#include <Ether.h>

int ether_texture_load(char const* picturePath, EtherTexture* id)
{
	int result = -1;
	int width, height, nrChannels;
	unsigned char* pictureContent = NULL;
		
	if (picturePath == NULL || id == NULL)
		return -1;
	
	// load and generate the texture
	pictureContent = stbi_load(picturePath, &width, &height, &nrChannels, 0);
	if (pictureContent == NULL)
		return -1;

	glGenTextures(1, id);
	glBindTexture(GL_TEXTURE_2D, *id);

	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pictureContent);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(pictureContent);

	return 0;
}

int ether_texture_unload(EtherTexture id)
{
	return 0;
}
