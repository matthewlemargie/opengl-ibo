#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>
#include <glm/gtx/string_cast.hpp>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <GL/gl.h>

#include "GLContext.h"
#include "Mesh.h"
#include "Camera.h"
#include "shaderClass.h"
#include "Scene.h"
#include "block.h"
#include "GenerateRandoms.h"

int main(void)
{
    GLContext glContext;

    float scale = 10.0f;

    Camera camera(glContext.window, glContext.mode->width, glContext.mode->height, glm::vec3(4.0f, 0.0f, 0.0f), 60.0f, 1.0f, 5000.0f);
    Scene scene(&glContext);
    Shader shader("default.vert", "default.frag");
    Shader lightShader("light.vert", "light.frag");
    // Shader rayShader("ray.vert", "ray.frag");

    Mesh lightCube("box.obj", lightShader, scale);
    vector<glm::mat4> lightModel = {glm::mat4(1.0f)};
    scene.addObject(lightCube);
    lightCube.addInstance(lightModel);

    Mesh box("box.obj", shader, scale);
    glm::mat4 transform = glm::mat4(1.0f);
    std::vector<glm::mat4> transforms = {};
    for (int i = 0; i < 1000; ++i) {
        transform = glm::translate(glm::mat4(1.0f), 500.0f * glm::vec3(generateFromNormal(), generateFromNormal(), generateFromNormal()));
        transforms.emplace_back(transform);
    }
    scene.addObject(box);
    box.addInstance(transforms);

    Block block(scale);
    transform = glm::mat4(1.0f);
    transforms.clear();
    for (int i = 0; i < 5000; ++i) {
        transform = glm::translate(glm::mat4(1.0f), 500.0f * glm::vec3(generateFromNormal(), generateFromNormal(), generateFromNormal()));
        transforms.emplace_back(transform);
    }
    scene.addBlock(block);
    block.addInstance(transforms);

    while (glContext.isWindowOpen())
    {
        glContext.fpsCounter.outputFPS();
        scene.Render(&camera);
        glfwSwapBuffers(glContext.window);
        glfwPollEvents();
    }
	shader.Delete();
    // rayShader.Delete();

    glfwTerminate();
    return 0;
}


