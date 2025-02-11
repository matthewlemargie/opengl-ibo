#include "Scene.h"

Scene::Scene(struct GLContext* GLContext)
: glContext(GLContext), wireframetoggler(GLContext->window), shader("shaders/default_vert.glsl", "shaders/default_frag.glsl"), lightShader("shaders/light_vert.glsl", "shaders/light_frag.glsl") 
{
    if (!GLContext) {
        std::cerr << "Error: GLContext is null!" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Create simple scene for rendering
    glm::mat4 transform;
    std::vector<glm::mat4> transforms(int(float(100000)/std::powf(2, 0))); // Pre-allocate space for transforms
    // std::vector<glm::mat4> transforms(int(float(std::powf(2, 16))/std::powf(2, 0))); // Pre-allocate space for transforms
    int numThreads = 4; // Number of threads to use
    int chunkSize = transforms.size() / numThreads; // Split work into chunks

    std::vector<std::thread> threads;

    double loadStartTime = glfwGetTime();

    Block* block = new Block(scale);
    for (int t = 0; t < numThreads; ++t) {
        int start = t * chunkSize;
        int end = (t == numThreads - 1) ? transforms.size() : (t + 1) * chunkSize; // Handle remainder in last thread

        threads.emplace_back([&transforms, start, end]() {
            glm::mat4 transform;
            for (int i = start; i < end; ++i) {
                transform = glm::translate(glm::mat4(1.0f), 1000.0f * glm::vec3(generateFromNormal(), generateFromNormal(), generateFromNormal()));
                transforms[i] = transform;
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    double loadTotalTime = glfwGetTime() - loadStartTime;
    cout << "# of threads: " << numThreads << endl;
    cout << "Total load time: " << loadTotalTime << "s" << endl;

    addBlock(*block);
    double vaoInitStartTime = glfwGetTime();
    block->addInstance(transforms);
    double vaoInitTotalTime = glfwGetTime() - vaoInitStartTime;
    cout << "Added instances to block IBO in " << vaoInitTotalTime << "s" << endl;
}


void Scene::addObject(Mesh& mesh) {
    Mesh* meshPtr = &mesh;
    meshes.insert(meshPtr);
}

void Scene::addBlock(Block& block) {
    Block* blockPtr = &block;
    blocks.insert(blockPtr);
}

void Scene::Render(Camera* camera) {
    timeValue = glfwGetTime();
    lightPos = 10.0f * glm::vec3(sin(timeValue), 0.5f * sin(8.0f * timeValue), cos(timeValue));
    // scale = 10.0f + abs(cos(2.0f * timeValue));

    camera->Inputs();
    camera->updateMatrix();


    skybox.Draw(camera);
    wireframetoggler.toggleWireframe();

    if (wireframetoggler.wireframeMode)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

// Mesh* lightCube = new Mesh("box.obj", lightShader, scale);
// addObject(*lightCube);
// lightCube->addInstance(glm::mat4(1.0f));

// Mesh* box = new Mesh("box.obj", shader, scale);
// glm::mat4 transform = glm::mat4(1.0f);
// for (int i = 0; i < 1000; ++i) {
    // transform = glm::translate(glm::mat4(1.0f), 500.0f * glm::vec3(generateFromNormal(), generateFromNormal(), generateFromNormal()));
    // transforms.emplace_back(transform);
// }
// addObject(*box);
// box->addInstance(transforms);
