#pragma once
#include <glm/glm.hpp>
#include "Camera.h"

glm::vec3 calculateRayWorldFromNDC(float xNDC, float yNDC, Camera camera);
glm::vec3 calculateRayWorld(double mouseX, double mouseY, Camera camera, GLFWvidmode mode);
glm::vec3 calculateRayWorldFromCamera(float xNDC, double yNDC, glm::mat4 projection, glm::mat4 view);
