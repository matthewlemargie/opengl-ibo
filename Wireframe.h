#ifndef WIREFRAME_CLASS_H
#define WIREFRAME_CLASS_H

#include <GLFW/glfw3.h>

class WireframeToggler {
public:
    WireframeToggler(GLFWwindow* window);

    void toggleWireframe();

private:
    GLFWwindow* window;
    bool wireframeMode;
    bool keyPressedLastFrame;
};

#endif
