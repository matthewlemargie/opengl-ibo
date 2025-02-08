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

    // initialize meshes/camera/shaders/scene/lights
    float scale = 10.0f;

    Camera camera(glContext.window, glContext.mode->width, glContext.mode->height, glm::vec3(4.0f, 0.0f, 0.0f), 60.0f, 1.0f, 5000.0f);

    Scene scene(&glContext, &camera);

    Shader shader("default.vert", "default.frag");
    Shader faceShader("face.vert", "face.frag");
    // Shader rayShader("ray.vert", "ray.frag");

    glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec4 lightColor = glm::vec4(0.5f, 0.7f, 0.4f, 1.0f);

    Mesh box("box.obj", shader, scale, lightColor, lightPos);
    glm::mat4 transform = glm::mat4(1.0f);
    std::vector<glm::mat4> transforms = {};
    for (int i = 0; i < 1000; ++i) {
        transform = glm::translate(glm::mat4(1.0f), 500.0f * glm::vec3(generateFromNormal(), generateFromNormal(), generateFromNormal()));
        transforms.emplace_back(transform);
    }
    // scene.addObject(box);
    // box.addInstance(transforms);

    Block block(scale, lightColor, lightPos);
    transform = glm::mat4(1.0f);
    transforms.clear();
    for (int i = 0; i < 5000; ++i) {
        transform = glm::translate(glm::mat4(1.0f), 500.0f * glm::vec3(generateFromNormal(), generateFromNormal(), generateFromNormal()));
        transforms.emplace_back(transform);
    }
    scene.addBlock(block);
    block.addInstance(transforms);

    float timeValue = 0;
    float phase = 2.0f * 0.015625f;

	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glDepthFunc(GL_LESS);
    // glEnable(GL_CULL_FACE);  // Enable face culling
    // glCullFace(GL_FRONT);      // Cull back faces (default)
    // glFrontFace(GL_CCW);      // Define front faces as counterclockwise (CCW)

    typedef void (*GLSwapIntervalFunc)(int);
    GLSwapIntervalFunc wglSwapIntervalEXT = (GLSwapIntervalFunc)glfwGetProcAddress("wglSwapIntervalEXT");
    if (wglSwapIntervalEXT) {
        wglSwapIntervalEXT(1);
    }

    glfwSwapInterval(1);

    while (glContext.isWindowOpen())
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// timeValue = glfwGetTime();
		// lightPos = 10.0f * glm::vec3(sin(timeValue), 0.5f * sin(8.0f * timeValue), cos(timeValue));
        // scale = 10.0f + abs(cos(2.0f * timeValue));

        scene.Render();

        glfwSwapBuffers(glContext.window);
        glfwPollEvents();
    }
	shader.Delete();
    faceShader.Delete();
    // rayShader.Delete();

    glfwTerminate();
    return 0;
}

//Mesh lightCube("C:/Users/matth/Downloads/box.obj");
//Shader lightShader("light.vert", "light.frag");
//vector<glm::mat4> lightModel = {glm::mat4(1.0f)};
//scene.addObject(lightCube, lightShader, lightModel);

