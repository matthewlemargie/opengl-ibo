#define GLM_ENABLE_EXPERIMENTAL
#ifndef IBO_CLASS_H
#define IBO_CLASS_H

#include <GL/glew.h>
#include <iostream>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <vector>

#include "VBO.h"

struct IBO
{
	GLuint ID;
	int numInstances = 0;
	int maxInstances = 1000;
	IBO();

	void addInstance(std::vector<glm::mat4>& instanceMats);
	void Bind();
	void Unbind();
	void Delete();
};

#endif
