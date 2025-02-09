#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include "stb/stb_image.h"

#include "shaderClass.h"

struct Texture
{
	GLuint ID;
	GLenum type;
	Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);

	void texUnit(Shader shader, const char* uniform, GLuint unit);
	void Bind();
	void Unbind();
	void Delete();

};

#endif
