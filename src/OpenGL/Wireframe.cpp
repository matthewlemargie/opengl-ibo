#include "Wireframe.h"
#include <GLFW/glfw3.h>
#include <GL/gl.h>

WireframeToggler::WireframeToggler(GLFWwindow* window)
    : window(window), wireframeMode(false), keyPressedLastFrame(false) {}

void WireframeToggler::toggleWireframe() {
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        if (!keyPressedLastFrame) {
            if (!wireframeMode) {
                wireframeMode = true;
            } else {
                wireframeMode = false;
            }

            keyPressedLastFrame = true;
        }
    } else {
        keyPressedLastFrame = false;
    }
}

