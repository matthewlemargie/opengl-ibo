#include "IBO.h"

IBO::IBO()
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * maxInstances, nullptr, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void IBO::addInstance(std::vector<glm::mat4>& instanceMats, AABB modelAABB) {
    if (instances.size() + instanceMats.size() > maxInstances) {
        return;
    }

    for (const auto& instanceMat : instanceMats) {
        glm::vec3 newMin = instanceMat * glm::vec4(modelAABB.min, 1.0f);
        glm::vec3 newMax = instanceMat * glm::vec4(modelAABB.max, 1.0f);
        AABB newAABB(newMin, newMax);
        aabbs.push_back(newAABB);
    }

    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * instances.size(), sizeof(glm::mat4) * instanceMats.size(), instanceMats.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    instances.insert(instances.end(), instanceMats.begin(), instanceMats.end());
}

void IBO::deleteInstance(int idx) {
    if (idx < 0 || idx >= instances.size()) {
        return;
    }

    // Move last instance into the deleted spot (swap-and-pop)
    glm::mat4 lastInstance = instances.back();
    instances[idx] = lastInstance;
    instances.pop_back();
    aabbs[idx] = aabbs.back();
    aabbs.pop_back();

    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * idx, sizeof(glm::mat4), &instances[idx]);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void IBO::updateBuffer() {
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * instances.size(), instances.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
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
