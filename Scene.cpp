#include "Scene.h"

Scene::Scene(struct GLContext* GLContext, Camera* camera) 
: glContext(GLContext), sceneCam(camera), skybox(*camera), wireframetoggler(GLContext->window) {}

void Scene::addObject(Mesh& mesh) {
    Mesh* meshPtr = &mesh;
    meshes.insert(meshPtr);
}

void Scene::addBlock(Block& block) {
    Block* blockPtr = &block;
    blocks.insert(blockPtr);
}

void Scene::Render() {
    glContext->fpsCounter.outputFPS();

    sceneCam->Inputs();
    sceneCam->updateMatrix();

    skybox.Draw();

    wireframetoggler.toggleWireframe();
    for (auto& mesh : meshes) {
        deleteMeshInstanceByRay(*mesh, glContext->window, glContext->mode, sceneCam);
        mesh->Draw(*sceneCam);
    }
    for (auto& block : blocks) {
        deleteBlockInstanceByRay(*block, glContext->window, glContext->mode, sceneCam);
        block->drawCube(*sceneCam);
    }
}
