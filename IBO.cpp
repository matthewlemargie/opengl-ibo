#include "IBO.h"

IBO::IBO()
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * 100000, nullptr, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void IBO::addInstance(std::vector<glm::mat4>& instanceMats)
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * (numInstances % maxInstances), sizeof(glm::mat4) * instanceMats.size(), instanceMats.data());
    numInstances += instanceMats.size();
}

void IBO::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void IBO::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void IBO::Delete()
{
	glDeleteBuffers(1, &ID);
}
