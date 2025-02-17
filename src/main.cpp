#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <chrono>
#include <thread>

#include "OpenGL/GLContext.h"
#include "OpenGL/Camera.h"

#include "world.h"
#include "OpenGL/Wireframe.h"

int main()
{
    double start = glfwGetTime();
    // Init gl context before doing anything in gl (or else you get dreaded segmentation fault)
    GLContext glContext;
    Camera camera(&glContext, glm::vec3(8.0f, 17.0f, 8.0f), 60.0f, 1.0f, 1000.0f);
    // Scene scene(&glContext);
    double timeToPrepare = glfwGetTime() - start;
    cout << "Scene prepared in " << timeToPrepare << "s" << endl;

    World world(&glContext);

    const double targetFPS = glContext.mode->refreshRate;
    const double frameTime = 1.0 / targetFPS;
    WireframeToggler wf(glContext.window);

    while (glContext.isWindowOpen())
    {
        auto startTime = std::chrono::high_resolution_clock::now();
        // glContext.fpsCounter.outputFPS();

        // rendering
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        wf.toggleWireframe();
        world.Render(camera);
        glfwSwapBuffers(glContext.window);
        glfwPollEvents();

        // Limit FPS
        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = endTime - startTime;
        double sleepTime = frameTime - elapsed.count();
        if (sleepTime > 0) {
            std::this_thread::sleep_for(std::chrono::duration<double>(sleepTime));
        }
    }

    glfwTerminate();
    return 0;
}

// Camera camera2(&glContext, glm::vec3(-4.0f, 0.0f, 0.0f), 60.0f, 1.0f, 5000.0f);
// if (glfwGetKey(glContext.window, GLFW_KEY_E) == GLFW_PRESS)
// {
    // scene.Render(&camera2);
    // camera.drawFrustumFromCamera(camera2);
// }
// else {
    // camera2.drawFrustumFromCamera(camera);
// }
