#include "Scene.h"

Scene::Scene(Camera* camera) : sceneCam(camera) {}

void Scene::addObject(Mesh& mesh, glm::mat4 modelMatrix) {
    std::vector<glm::mat4> model;
    model.emplace_back(modelMatrix);
    mesh.addInstance(model);

    Mesh* meshPtr = &mesh;

    if (meshes.find(meshPtr) == meshes.end()) {
        meshes.insert(meshPtr);
    }
}

void Scene::addObjects(Mesh& mesh, std::vector<glm::mat4> modelMatrices) {
    mesh.addInstance(modelMatrices);
    Mesh* meshPtr = &mesh;
    meshes.insert(meshPtr);
}

void Scene::Render(GLFWwindow* window, const GLFWvidmode* mode) {
    for (auto& mesh : meshes) {
        deleteMeshInstanceByRay(*mesh, window, mode, sceneCam);
        mesh->Draw(*sceneCam);
    }
}
