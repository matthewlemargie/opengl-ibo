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
    float scale = 5.0f;
    Mesh box("box.obj", scale);

    Camera camera(mode->width, mode->height, glm::vec3(4.0f, 0.0f, 0.0f), 90.0f, 1.0f, 5000.0f);

    Shader shader("default.vert", "default.frag");
    Shader rayShader("ray.vert", "ray.frag");
    Shader faceShader("face.vert", "face.frag");

    Scene scene(&camera);

    glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec4 lightColor = glm::vec4(0.5f, 0.7f, 0.4f, 1.0f);

    glm::mat4 transform = glm::mat4(1.0f);

    float timeValue = 0;
    float phase = 2.0f * 0.015625f;

	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glDepthFunc(GL_LESS);

    glfwSwapInterval(1);
    
    WireframeToggler wireframetoggler(window);
    FPSCounter fpsCounter;

    scene.addObject(box, shader, transform);
    std::vector<glm::mat4> transforms = {};
    // if (box.ibo.numInstances < box.ibo.maxInstances) {
    for (int i = 0; i < 1000; ++i) {
        transform = glm::translate(glm::mat4(1.0f), 500.0f * glm::vec3(generateFromNormal(), generateFromNormal(), generateFromNormal()));
        transforms.emplace_back(transform);
    }
    // }
    box.addInstance(transforms);

    GLfloat faceVertices[] = {
        // position                normals                  UVs
        -10.0f, 10.0f, 0.0f,       0.0f, 0.0f, 1.0f,        0.0f, 1.0f, // top left 
        10.0f, 10.0f, 0.0f,        0.0f, 0.0f, 1.0f,        1.0f, 1.0f, // top right 
        -10.0f, -10.0f, 0.0f,      0.0f, 0.0f, 1.0f,        0.0f, 0.0f, // bottom left
        10.0f, -10.0f, 0.0f,       0.0f, 0.0f, 1.0f,        1.0f, 0.0f, // bottom right
    };

    GLuint faceIndices[] = {
        0, 1, 2,
        3, 1, 2, 
    };


    Mesh face(faceVertices, sizeof(faceVertices), faceIndices, sizeof(faceIndices));
    transform = glm::mat4(1.0f);
    transforms.clear();
    scene.addFace(face, faceShader, transform);
    for (int i = 0; i < 1000; ++i) {
        transform = glm::translate(glm::mat4(1.0f), 500.0f * glm::vec3(generateFromNormal(), generateFromNormal(), generateFromNormal()));
        transforms.emplace_back(transform);
    }
    face.addFaceInstance(transforms);

    Texture grass("grass.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    grass.texUnit(shader, "tex0", 0);
    grass.Bind();

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // fpsCounter.outputFPS();
        wireframetoggler.toggleWireframe(window);

		timeValue = glfwGetTime();
		lightPos = 10.0f * glm::vec3(sin(timeValue), 0.5f * sin(8.0f * timeValue), cos(timeValue));
        scale = 5.0f + abs(1.0f * cos(2.0f * timeValue));

        camera.Inputs(window);
        camera.updateMatrix();

        scene.Render(window, mode, camera, lightPos, lightColor, scale);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
	shader.Delete();
    rayShader.Delete();
    faceShader.Delete();

    glfwTerminate();
    return 0;
}

//Mesh lightCube("C:/Users/matth/Downloads/box.obj");
//Shader lightShader("light.vert", "light.frag");
//vector<glm::mat4> lightModel = {glm::mat4(1.0f)};
//scene.addObject(lightCube, lightShader, lightModel);

