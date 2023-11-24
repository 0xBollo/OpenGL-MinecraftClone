#include "Texture.hpp"



Texture::Texture(const char *imgPath)
{
	// Create and bind texture
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	// Configure texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load image and generate texture
	int width, height;
	stbi_set_flip_vertically_on_load(true);
	GLubyte *img = stbi_load(imgPath, &width, &height, 0, STBI_rgb);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Free image memory and unbind texture
	stbi_image_free(img);
	glBindTexture(GL_TEXTURE_2D, 0);
}


void Texture::bind() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, id);
}


void Texture::bindToTexUnit(GLenum texUnit) const
{
	glActiveTexture(texUnit);
	glBindTexture(GL_TEXTURE_2D, id);
}