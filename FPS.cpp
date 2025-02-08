#include "FPS.h"

void FPSCounter::outputFPS() {
    printf("\x1b[d");
    printf("\x1b[2J");
    currTime = glfwGetTime();
    timeDiff = currTime - prevTime;
    counter++;
    if (timeDiff >= 1.0 / 30.0) {
        double FPS = (1.0 / timeDiff) * counter;
        std::cout << "FPS: " << FPS << std::endl;
        prevTime = currTime;
        counter = 0;
    }
};
