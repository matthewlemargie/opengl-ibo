#ifndef GLCONTEXT_CLASS_H
#define GLCONTEXT_CLASS_H

#include <GL/glew.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "FPS.h"

struct GLContext {
    GLFWwindow* window;
    GLFWmonitor* monitor;
    const GLFWvidmode* mode;
    FPSCounter fpsCounter;

    GLContext();

    ~GLContext();

    bool isWindowOpen() const;
};

#endif
