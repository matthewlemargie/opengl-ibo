#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "GLContext.h"
#include "Camera.h"
#include "Scene.h"

int main()
{
    GLContext glContext;
    Camera camera(&glContext, glm::vec3(4.0f, 0.0f, 0.0f), 60.0f, 1.0f, 5000.0f);
    Camera camera2(&glContext, glm::vec3(-4.0f, 0.0f, 0.0f), 90.0f, 1.0f, 5000.0f);
    Scene scene(&glContext);

    while (glContext.isWindowOpen())
    {
        glContext.fpsCounter.outputFPS();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if (glfwGetKey(glContext.window, GLFW_KEY_E) == GLFW_PRESS)
            scene.Render(&camera2);
        else
            scene.Render(&camera);
        glfwSwapBuffers(glContext.window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}


