#pragma once

#include <glad/glad.h>

#include "stb_image.h"



class Texture
{
	private:
		GLuint id;

	public:
		Texture() { id = 0; }

		// Read the given image and generate and configure the texture
		Texture(const char *imgPath);

		// Bind this texture
		void bind() const;
		void bindToTexUnit(GLenum texUnit) const;
};