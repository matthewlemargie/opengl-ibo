#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>
#include <glm/gtx/string_cast.hpp>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <GL/gl.h>

#include "GLContext.h"
#include "Mesh.h"
#include "Camera.h"
#include "Scene.h"

int main(void)
{
    GLContext glContext;
    Camera camera(&glContext, glm::vec3(4.0f, 0.0f, 0.0f), 60.0f, 1.0f, 5000.0f);
    Scene scene(&glContext);

    while (glContext.isWindowOpen())
    {
        glContext.fpsCounter.outputFPS();
        scene.Render(&camera);
        glfwSwapBuffers(glContext.window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}


