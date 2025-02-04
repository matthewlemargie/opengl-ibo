#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>
#include <glm/gtx/string_cast.hpp>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <iostream>
#include <random>

#include "Wireframe.h"
#include "calculateRays.h"
#include "ModelLoader.h"
#include "Mesh.h"
#include "Camera.h"
#include "shaderClass.h"
#include "Scene.h"

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

int main(void)
{
    GLFWwindow* window;

    // Initialize the library
    if (!glfwInit())
        return -1;

    unsigned int samples = 8;

    glfwWindowHint(GLFW_SAMPLES, samples);

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
    // Mesh woman("C:/Users/matth/Downloads/woman_low.obj", "C:/Users/matth/Downloads/woman_med.obj", "C:/Users/matth/Downloads/woman_high.obj");
    Mesh box("box.obj");

    Camera camera(mode->width, mode->height, glm::vec3(4.0f, 0.0f, 0.0f), 90.0f, 1.0f, 1000.0f);
    Camera camera2(mode->width, mode->height, glm::vec3(4.0f, 0.0f, 0.0f), 45.0f, 1.0f, 200.0f);

    Shader shader("default.vert", "default.frag");
    Shader rayShader("ray.vert", "ray.frag");

    Scene scene(&camera);

    glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec4 lightColor = glm::vec4(0.5f, 0.7f, 0.4f, 1.0f);

    glm::mat4 transform = glm::mat4(1.0f);

    float scale;
    float timeValue = 0;
    float phase = 2.0f * 0.015625f;

	scene.addObject(box, shader, transform);

	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glDepthFunc(GL_LESS);

    double prevTime = 0.0;
    double currTime = 0.0;
    double timeDiff;
    unsigned int counter = 0;

    glfwSwapInterval(1);
    
    WireframeToggler wireframetoggler(window);
    
    Ray ray;

    while (!glfwWindowShouldClose(window))
    {
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

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        wireframetoggler.toggleWireframe(window);

		timeValue = glfwGetTime();
		lightPos = 10.0f * glm::vec3(sin(timeValue), 0.5f * sin(8.0f * timeValue), cos(timeValue));
        scale = 5.0f + abs(1.0f * cos(2.0f * timeValue));

        std::vector<glm::mat4> transforms = {};
        if (box.ibo.numInstances < box.ibo.maxInstances)
        {
            for (int i = 0; i < 1; ++i) {
                transform = glm::translate(glm::mat4(1.0f), 500.0f * glm::vec3(generateFromNormal(), generateFromNormal(), generateFromNormal()));
                transforms.emplace_back(transform);
            }
        }
        box.addInstance(transforms);

        camera.Inputs(window);
        camera.updateMatrix();

        ray.drawRay(window, &camera, mode, &rayShader);
        
        scene.Render(camera, lightPos, lightColor, scale);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
	shader.Delete();
    rayShader.Delete();

    glfwTerminate();
    return 0;
}

//Mesh lightCube("C:/Users/matth/Downloads/box.obj");
//Shader lightShader("light.vert", "light.frag");
//vector<glm::mat4> lightModel = {glm::mat4(1.0f)};
//scene.addObject(lightCube, lightShader, lightModel);

