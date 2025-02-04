#include "calculateRays.h"

glm::vec3 Ray::calculateRayWorldFromNDC(float xNDC, float yNDC, Camera camera) {
    glm::vec4 rayClip(xNDC, yNDC, -1.0f, 1.0f);

    glm::mat4 inverseProjection = glm::inverse(camera.projection);
    glm::vec4 rayView = inverseProjection * rayClip;
    rayView.z = -1.0f;
    rayView.w = 0.0f;

    glm::mat4 inverseView = glm::inverse(camera.view);
    glm::vec3 rayWorld = glm::normalize(glm::vec3(inverseView * rayView));

    return rayWorld;
}

void Ray::calculateRayWorld(GLFWwindow* window, const Camera* camera, const GLFWvidmode* mode){
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);
	float xNDC = ((2.0f * mouseX) / mode->width - 1.0f);
	float yNDC = 1.0f - (2.0f * mouseY) / mode->height;
	glm::vec4 rayClip(xNDC, yNDC, -1.0f, 1.0f);

	glm::mat4 inverseProjection = glm::inverse(camera->projection);
	glm::vec4 rayView = inverseProjection * rayClip;
	rayView.z = -1.0f;
	rayView.w = 0.0f;

	glm::mat4 inverseView = glm::inverse(camera->view);
	glm::vec3 rayWorld = glm::normalize(glm::vec3(inverseView * rayView));

    glm::vec3 lineVertices[] = {
        camera->Position,
        camera->Position + rayWorld * camera->farPlane
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Upload vertex data
    glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_STATIC_DRAW);

    // Define vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Ray::drawRay(GLFWwindow* window, Camera* camera, const GLFWvidmode* mode, Shader* shader) {
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        resetRay();

    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
        calculateRayWorld(window, camera, mode);
    if (VAO != 0) {
        shader->Activate();
        camera->Matrix(*shader, "camMatrix");
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, 2);
        glBindVertexArray(0);
    }
}

void Ray::resetRay() {
    VAO = 0;
}

glm::vec3 Ray::calculateRayWorldFromCamera(float xNDC, double yNDC, glm::mat4 projection, glm::mat4 view) {
    glm::vec4 rayClip(xNDC, yNDC, -1.0f, 1.0f);

    glm::mat4 inverseProjection = glm::inverse(projection);
    glm::vec4 rayView = inverseProjection * rayClip;
    rayView.z = -1.0f;
    rayView.w = 0.0f;

    glm::mat4 inverseView = glm::inverse(view);
    glm::vec3 rayWorld = glm::normalize(glm::vec3(inverseView * rayView));

    return rayWorld;
}
