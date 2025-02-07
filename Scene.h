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

struct Scene {
    std::set<Mesh*> meshes;
    Camera* sceneCam;

    Scene(Camera* camera);

    void addObject(Mesh& mesh, glm::mat4 modelMatrix);
    void addObjects(Mesh& mesh, std::vector<glm::mat4> modelMatrices);
	void Render(GLFWwindow* window, const GLFWvidmode* mode);
};
