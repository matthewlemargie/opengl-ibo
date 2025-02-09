#define GLM_ENABLE_EXPERIMENTAL
#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <GL/glew.h>
#include <iostream>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <vector>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texUV;
};

struct VBO
{
	GLuint ID;
	VBO();

	void addVertices(std::vector<Vertex>& vertices);
    void addVertices(GLfloat* vertices, size_t size);
	void Bind();
	void bindInstance();
	void Unbind();
	void Delete();
};

#endif
