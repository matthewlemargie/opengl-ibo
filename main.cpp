#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>
#include <glm/gtx/string_cast.hpp>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <iostream>
#include <random>

#include "VBO.h"
#include "Wireframe.h"
#include "calculateRays.h"
#include "ModelLoader.h"
#include "Mesh.h"
#include "Camera.h"
#include "shaderClass.h"
#include "Scene.h"
#include "block.h"
#include "skybox.h"

float generateRandomFloat() {
    // Create a random device and use it to seed the random number generator
    std::random_device rd;
    std::mt19937 gen(rd());  // Mersenne Twister engine

    // Create a uniform distribution in the desired range
    std::uniform_real_distribution<> dis(-1.0f, 1.0f);

    // Generate and return the random float
    return dis(gen);
}

float generateFromNormal() {
    // Create a random device and use it to seed the random number generator
    std::random_device rd;
    std::mt19937 gen(rd());  // Mersenne Twister engine

    // Create a normal distribution in the desired range
    std::normal_distribution<> dis(0.0f, 1.0f);

    // Generate and return the random float
    return dis(gen);
}

struct FPSCounter {
    double prevTime = 0.0;
    double currTime = 0.0;
    double timeDiff;
    unsigned int counter = 0;

    void outputFPS() {
        printf("\x1b[d");
        printf("\x1b[2J");
        currTime = glfwGetTime();
        timeDiff = currTime - prevTime;
        counter++;
        if (timeDiff >= 1.0 / 30.0) {
            double FPS = (1.0 / timeDiff) * counter;
            cout << "FPS: " << FPS << endl;
            prevTime = currTime;
            counter = 0;
        }
    };
};

struct GLContext {
    GLFWwindow* window;
    GLFWmonitor* monitor;
    const GLFWvidmode* mode;
    GLContext() {}
};

int main(void)
{
    GLFWwindow* window;

    // Initialize the library
    if (!glfwInit())
        return -1;

    unsigned int samples = 32;

    glfwWindowHint(GLFW_SAMPLES, samples);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);

    // initiliaze monitor/window/mode
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    if (!monitor) {
        std::cerr << "Failed to get primary monitor!" << std::endl;
        glfwTerminate();
        return -1;
    }

    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    if (!mode) {
        std::cerr << "Failed to get video mode!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    window = glfwCreateWindow(mode->width, mode->height, "OpenGL Window", monitor, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glViewport(0, 0, mode->width, mode->height);

    // initilize GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW!" << std::endl;
        return -1;
    }

    // initialize meshes/camera/shaders/scene/lights
    float scale = 10.0f;

    Camera camera(mode->width, mode->height, glm::vec3(4.0f, 0.0f, 0.0f), 60.0f, 1.0f, 5000.0f);

    Scene scene(window, mode, &camera);

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

    WireframeToggler wireframetoggler(window);
    FPSCounter fpsCounter;

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // fpsCounter.outputFPS();
        wireframetoggler.toggleWireframe(window);

		timeValue = glfwGetTime();
		lightPos = 10.0f * glm::vec3(sin(timeValue), 0.5f * sin(8.0f * timeValue), cos(timeValue));
        scale = 10.0f + abs(cos(2.0f * timeValue));

        camera.Inputs(window);
        camera.updateMatrix();

        scene.Render();

        glfwSwapBuffers(window);
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

