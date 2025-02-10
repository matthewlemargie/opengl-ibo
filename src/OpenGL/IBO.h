#define GLM_ENABLE_EXPERIMENTAL
#ifndef IBO_CLASS_H
#define IBO_CLASS_H

#include <GL/glew.h>
#include <iostream>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <vector>

#include "VBO.h"
#include "../AABB.h"

struct IBO
{
	GLuint ID;
	int maxInstances = 1000000;
    std::vector<glm::mat4> instances;
    std::vector<AABB> aabbs;

	IBO();

    void addInstance(std::vector<glm::mat4>& instanceMats, AABB modelAABB);
    void addInstance(glm::mat4& instanceMat, AABB modelAABB);
    void addInstance(std::vector<glm::mat4>& instanceMats);
    void addInstance(glm::mat4& instanceMat);
    void deleteInstance(int idx);
    void updateBuffer();
	void Bind();
	void Unbind();
	void Delete();
};

#endif
