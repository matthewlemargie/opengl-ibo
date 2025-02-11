#include "GLContext.h"

GLContext::GLContext() : window(nullptr), monitor(nullptr), mode(nullptr) {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        return;
    }
    // Initialize monitor and mode
    monitor = glfwGetPrimaryMonitor();
    if (!monitor) {
        std::cerr << "Failed to get primary monitor!" << std::endl;
        glfwTerminate();
        return;
    }
    mode = glfwGetVideoMode(monitor);
    if (!mode) {
        std::cerr << "Failed to get video mode!" << std::endl;
        glfwTerminate();
        return;
    }
    // Set GLFW window hints
    unsigned int samples = 16;
    glfwWindowHint(GLFW_SAMPLES, samples);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
    // Create the GLFW window
    window = glfwCreateWindow(mode->width, mode->height, "OpenGL Window", monitor, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);
    glViewport(0, 0, mode->width, mode->height);
    glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    glewExperimental = GL_TRUE;
    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW!" << std::endl;
        glfwTerminate();
        return;
    }

	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glDepthFunc(GL_LESS);
    // glEnable(GL_CULL_FACE);  // Enable face culling
    // glCullFace(GL_FRONT);      // Cull back faces (default)
    // glFrontFace(GL_CCW);      // Define front faces as counterclockwise (CCW)

    glfwSwapInterval(0);

    // double check MSAA antialiasing is working properly
    int msaa;
    glGetIntegerv(GL_SAMPLES, &msaa);
    std::cout << "MSAA Samples: " << samples << std::endl;
}

GLContext::~GLContext() {
    // Clean up GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool GLContext::isWindowOpen() const {
    return !glfwWindowShouldClose(window);
}

