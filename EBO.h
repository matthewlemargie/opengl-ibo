#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include <GL/glew.h>
#include <vector>

struct EBO
{
	GLuint ID;
	EBO(std::vector<GLuint>& indices);
	void Bind();
	void Unbind();
	void Delete();
};

#endif
