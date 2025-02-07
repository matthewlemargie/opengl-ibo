#pragma once
#include <glm/mat4x4.hpp>
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

struct Scene {
    std::set<Mesh*> meshes;
    std::set<Block*> blocks;
    GLFWwindow* sceneWindow;
    const GLFWvidmode* sceneMode;
    Camera* sceneCam;

    Scene(GLFWwindow* window, const GLFWvidmode* mode, Camera* camera);

    void addObject(Mesh& mesh);
    void addBlock(Block& block);
	void Render();
};
