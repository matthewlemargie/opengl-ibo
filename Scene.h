#pragma once
#include <vector>
#include <tuple>
#include "ModelLoader.h"
#include "Mesh.h"
#include "shaderClass.h"
#include "Camera.h"
#include "VAO.h"
#include "calculateRays.h"
#include <glm/mat4x4.hpp>
#include <map>
#include <set>

struct Scene {
    std::map<Shader*, std::set<Mesh*>> shaderMap;
    Camera* sceneCam;

    Scene(Camera* camera);

    void addObject(Mesh& mesh, Shader& shader, glm::mat4 modelMatrix);
	void Render(GLFWwindow* window, const GLFWvidmode* mode, Camera& camera, glm::vec3 lightPos, glm::vec4 lightColor, float scale);
};
