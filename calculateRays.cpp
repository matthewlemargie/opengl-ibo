#include "calculateRays.h"

glm::vec3 calculateRayWorldFromNDC(float xNDC, float yNDC, Camera camera) {
    glm::vec4 rayClip(xNDC, yNDC, -1.0f, 1.0f);

    glm::mat4 inverseProjection = glm::inverse(camera.projection);
    glm::vec4 rayView = inverseProjection * rayClip;
    rayView.z = -1.0f;
    rayView.w = 0.0f;

    glm::mat4 inverseView = glm::inverse(camera.view);
    glm::vec3 rayWorld = glm::normalize(glm::vec3(inverseView * rayView));

    return rayWorld;
}

glm::vec3 calculateRayWorld(double mouseX, double mouseY, Camera camera, GLFWvidmode mode) {
	float xNDC = ((2.0f * mouseX) / mode.width - 1.0f);
	float yNDC = 1.0f - (2.0f * mouseY) / mode.height;
	glm::vec4 rayClip(xNDC, yNDC, -1.0f, 1.0f);

	glm::mat4 inverseProjection = glm::inverse(camera.projection);
	glm::vec4 rayView = inverseProjection * rayClip;
	rayView.z = -1.0f;
	rayView.w = 0.0f;

	glm::mat4 inverseView = glm::inverse(camera.view);
	glm::vec3 rayWorld = glm::normalize(glm::vec3(inverseView * rayView));

    return rayWorld;
}
glm::vec3 calculateRayWorldFromCamera(float xNDC, double yNDC, glm::mat4 projection, glm::mat4 view) {
    glm::vec4 rayClip(xNDC, yNDC, -1.0f, 1.0f);

    glm::mat4 inverseProjection = glm::inverse(projection);
    glm::vec4 rayView = inverseProjection * rayClip;
    rayView.z = -1.0f;
    rayView.w = 0.0f;

    glm::mat4 inverseView = glm::inverse(view);
    glm::vec3 rayWorld = glm::normalize(glm::vec3(inverseView * rayView));

    return rayWorld;
}
