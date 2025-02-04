#include "Wireframe.h"
#include <GLFW/glfw3.h>
#include <GL/gl.h>

WireframeToggler::WireframeToggler(GLFWwindow* window)
    : wireframeMode(false), keyPressedLastFrame(false) {}

void WireframeToggler::toggleWireframe(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        if (!keyPressedLastFrame) {
            if (!wireframeMode) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                wireframeMode = true;
            } else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                wireframeMode = false;
            }

            keyPressedLastFrame = true;
        }
    } else {
        keyPressedLastFrame = false;
    }
}

