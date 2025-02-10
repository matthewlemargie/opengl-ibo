#pragma once
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <thread>
#include <iostream>
#include <atomic>
#include <mutex>
#include "Camera.h"
#include "Mesh.h"
#include "../block.h"
#include "GLContext.h"

glm::vec3 calculateRayWorld(GLFWwindow* window, const Camera* camera, const GLFWvidmode* mode);
glm::vec3 calculateRayWorldFromNDC(float xNDC, float yNDC, Camera camera);
glm::vec3 calculateRayWorldFromCamera(float xNDC, double yNDC, glm::mat4 projection, glm::mat4 view);
void deleteMeshInstanceByRay(Mesh& mesh, GLFWwindow* window, const GLFWvidmode* mode, Camera* camera);
void deleteBlockInstanceByRay(Block& block, GLFWwindow* window, const GLFWvidmode* mode, Camera* camera);

struct Ray {
    GLuint VAO = 0;
    GLuint VBO;
    Shader rayShader;
    Ray();

    void createRayLine(glm::vec3 rayWorld, const Camera* camera);
    void drawRay(GLContext* glContext, Camera* camera);
    void resetRay();
};

