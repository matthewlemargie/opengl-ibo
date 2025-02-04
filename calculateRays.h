#pragma once
#include <glm/glm.hpp>
#include "Camera.h"

struct Ray {
    GLuint VAO = 0;
    GLuint VBO;

    glm::vec3 calculateRayWorldFromNDC(float xNDC, float yNDC, Camera camera);
    glm::vec3 calculateRayWorld(GLFWwindow* window, const Camera* camera, const GLFWvidmode* mode);
    void resetRay();
    void createRayLine(glm::vec3 rayWorld, const Camera* camera);
    void drawRay(GLFWwindow* window, Camera* camera, const GLFWvidmode* mode, Shader* shader);
    glm::vec3 calculateRayWorldFromCamera(float xNDC, double yNDC, glm::mat4 projection, glm::mat4 view);
};

