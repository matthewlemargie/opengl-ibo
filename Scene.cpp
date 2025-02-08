#include "Scene.h"

Scene::Scene(struct GLContext* GLContext) 
: glContext(GLContext), wireframetoggler(GLContext->window) {}

void Scene::addObject(Mesh& mesh) {
    Mesh* meshPtr = &mesh;
    meshes.insert(meshPtr);
}

void Scene::addBlock(Block& block) {
    Block* blockPtr = &block;
    blocks.insert(blockPtr);
}

void Scene::Render(Camera* camera) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    timeValue = glfwGetTime();
    lightPos = 10.0f * glm::vec3(sin(timeValue), 0.5f * sin(8.0f * timeValue), cos(timeValue));
    scale = 10.0f + abs(cos(2.0f * timeValue));

    camera->Inputs();
    camera->updateMatrix();

    skybox.Draw(camera);

    wireframetoggler.toggleWireframe();
    for (auto& mesh : meshes) {
        mesh->shader->Activate();
        glUniform1f(glGetUniformLocation(mesh->shader->ID, "scale"), *mesh->scale);
        glUniform4f(glGetUniformLocation(mesh->shader->ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
        glUniform3f(glGetUniformLocation(mesh->shader->ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
        camera->Matrix(*mesh->shader, "camMatrix");
        deleteMeshInstanceByRay(*mesh, glContext->window, glContext->mode, camera);
        mesh->Draw(*camera);
    }
    for (auto& block : blocks) {
        block->textureActivate(*camera);
        block->shader->Activate();
        glUniform1f(glGetUniformLocation(block->shader->ID, "scale"), *block->scale);
        glUniform4f(glGetUniformLocation(block->shader->ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
        glUniform3f(glGetUniformLocation(block->shader->ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
        camera->Matrix(*block->shader, "camMatrix");
        deleteBlockInstanceByRay(*block, glContext->window, glContext->mode, camera);
        block->drawCube();
    }
}
