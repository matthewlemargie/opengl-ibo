#ifndef WIREFRAME_CLASS_H
#define WIREFRAME_CLASS_H

#include <GLFW/glfw3.h>

struct WireframeToggler {
    WireframeToggler(GLFWwindow* window);

    void toggleWireframe(GLFWwindow* window);

    bool wireframeMode;
    bool keyPressedLastFrame;
};

#endif
