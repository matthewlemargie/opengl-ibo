#include "Scene.h"

Scene::Scene(GLFWwindow* window, const GLFWvidmode* mode, Camera* camera) : sceneWindow(window), sceneMode(mode), sceneCam(camera), skybox(*camera) {}

void Scene::addObject(Mesh& mesh) {
    Mesh* meshPtr = &mesh;
    meshes.insert(meshPtr);
}

void Scene::addBlock(Block& block) {
    Block* blockPtr = &block;
    blocks.insert(blockPtr);
}

void Scene::Render() {
    skybox.Draw();
    for (auto& mesh : meshes) {
        deleteMeshInstanceByRay(*mesh, sceneWindow, sceneMode, sceneCam);
        mesh->Draw(*sceneCam);
    }
    for (auto& block : blocks) {
        deleteBlockInstanceByRay(*block, sceneWindow, sceneMode, sceneCam);
        block->drawCube(*sceneCam);
    }
}
