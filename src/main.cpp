#define GLM_ENABLE_EXPERIMENTAL
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <chrono>
#include <thread>

#include "OpenGL/GLContext.h"
#include "OpenGL/Camera.h"
#include "OpenGL/Wireframe.h"

#include "OpenGL/skybox.h"
#include "world.h"
#include "constants.h"

int main()
{
    double start = glfwGetTime();
    // Init gl context before doing anything in gl (or else you get dreaded segmentation fault)
    GLContext glContext;
    Camera camera(&glContext, glm::vec3(0.0f, (float)CHUNK_Y_DIM + 1.0f, 0.0f), 60.0f, 1.0f, 1000.0f);
    // Scene scene(&glContext);
    double timeToPrepare = glfwGetTime() - start;
    cout << "Scene prepared in " << timeToPrepare << "s" << endl;

    Skybox skybox;
    World world(&glContext);

    const double targetFPS = glContext.mode->refreshRate;
    const double frameTime = 1.0 / targetFPS;
    WireframeToggler wf(glContext.window);
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); 
    (void)io;
    ImGui::StyleColorsDark();  // Optional: change theme

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(glContext.window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    bool vsync = true;
    bool running = true;

    std::thread chunkThread(std::bind(&World::updateChunks, &world, std::ref(camera), std::ref(running)));

    while (glContext.isWindowOpen())
    {
        auto startTime = std::chrono::high_resolution_clock::now();
        glContext.fpsCounter.outputFPS();

        // rendering
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        wf.toggleWireframe();
        // skybox.Draw(&camera);
        world.processChunks(camera);
        world.Render(camera);
        // Create a window and display variables
        ImGui::Begin("Variable Viewer");
        ImGui::Text("Camera position: %.3f, %.3f, %.3f", camera.Position.x, camera.Position.y, camera.Position.z);
        ImGui::Text("Camera orientation: %.3f, %.3f, %.3f", camera.Orientation.x, camera.Orientation.y, camera.Orientation.z);
        ImGui::Text("Framerate: %.3f", glContext.fpsCounter.FPS);
        ImGui::Checkbox("VSync", &vsync);
        ImGui::End();  // End the window

        // Render ImGui draw data
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(glContext.window);
        glfwPollEvents();
        // Limit FPS
        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = endTime - startTime;
        double sleepTime = frameTime - elapsed.count();
        if (sleepTime > 0 && vsync) {
            std::this_thread::sleep_for(std::chrono::duration<double>(sleepTime));
        }
    }

    glfwTerminate();
    running = false;
    chunkThread.join();
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
