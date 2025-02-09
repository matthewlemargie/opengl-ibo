#ifndef FPS_CLASS_H
#define FPS_CLASS_H

#include <stdio.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>

struct FPSCounter {
    double prevTime = 0.0;
    double currTime = 0.0;
    double timeDiff;
    unsigned int counter = 0;

    void outputFPS();
};

#endif
