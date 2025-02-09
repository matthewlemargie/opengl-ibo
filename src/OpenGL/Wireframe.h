#ifndef WIREFRAME_CLASS_H
#define WIREFRAME_CLASS_H

#include <GLFW/glfw3.h>

struct WireframeToggler {
    GLFWwindow* window;
    WireframeToggler(GLFWwindow* window);

    void toggleWireframe();

    bool wireframeMode;
    bool keyPressedLastFrame;
};

#endif
