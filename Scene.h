#pragma once
#include <glm/mat4x4.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <map>
#include <set>
#include <vector>
#include <tuple>

#include "ModelLoader.h"
#include "Mesh.h"
#include "shaderClass.h"
#include "Camera.h"
#include "VAO.h"
#include "calculateRays.h"
#include "block.h"
#include "skybox.h"
#include "Wireframe.h"
#include "GLContext.h"

struct Scene {
    std::set<Mesh*> meshes;
    std::set<Block*> blocks;

    glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec4 lightColor = glm::vec4(0.5f, 0.7f, 0.4f, 1.0f);

    float timeValue = glfwGetTime();
    float scale = 10.0f;

    Skybox skybox;
    WireframeToggler wireframetoggler;
    GLContext* glContext;

    Scene(struct GLContext* GLContext);

    void addObject(Mesh& mesh);
    void addBlock(Block& block);
	void Render(Camera* camera);
};
