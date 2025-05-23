#pragma once
#include <glm/mat4x4.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <set>
#include <vector>
#include <thread>

#include "GLContext.h"
#include "skybox.h"
#include "shaderClass.h"
#include "Wireframe.h"
#include "../GenerateRandoms.h"

struct Scene {
    GLContext* glContext;
    // World world;

    // Skybox skybox;

    Shader shader;
    Shader lightShader;

    WireframeToggler wireframetoggler;

    glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec4 lightColor = glm::vec4(0.5f, 0.7f, 0.4f, 1.0f);

    float timeValue;
    float scale = 10.0f;

    Scene(struct GLContext* GLContext);

	void Render(Camera* camera);
};
