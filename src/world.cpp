#include "world.h"

World::World(GLContext* context) {
    auto start = std::chrono::high_resolution_clock::now();

    shader = new Shader("shaders/block_vert.glsl", "shaders/block_frag.glsl");
    atlas = new Texture("assets/textures/grass.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);

    std::cout << "World initialized!" << std::endl;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Initialization took " << duration.count() << " seconds" << std::endl;
}

World::~World() {
    chunks.clear();
    for (auto& [pos, vao] : chunkVAOs) {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &chunkVBOs[pos]);
        glDeleteBuffers(1, &chunkEBOs[pos]);
    }
    delete atlas;
    delete shader;
}

void World::updateChunks(Camera& camera, bool& running) {
    while (running) {
        float worldOffsetX = (WORLD_X_DIM * CHUNK_X_DIM) / 2.0f;
        float worldOffsetZ = (WORLD_Z_DIM * CHUNK_Z_DIM) / 2.0f;

        int newChunkX = (static_cast<int>(camera.Position.x + worldOffsetX) / CHUNK_X_DIM);
        int newChunkZ = (static_cast<int>(camera.Position.z + worldOffsetZ) / CHUNK_Z_DIM);

        if (currentChunk.first != newChunkX || currentChunk.second != newChunkZ) {
            currentChunk = {newChunkX, newChunkZ};
            {
                std::lock_guard<std::mutex> lock(chunkMutex);
                loadChunksAround(newChunkX, newChunkZ);
            }
        }
        std::this_thread::sleep_for(std::chrono::duration<double>(0.1));
    }
}

void World::loadChunksAround(int centerX, int centerZ) {
    for (int i = -dataLoadRadius; i <= dataLoadRadius; ++i) {
        for (int j = -dataLoadRadius; j <= dataLoadRadius; ++j) {
            int chunkX = centerX + i;
            int chunkZ = centerZ + j;
            std::pair<int, int> chunkPos = {chunkX, chunkZ};

            // std::lock_guard<std::mutex> lock(chunkQueueMutex);

            // Push chunk to queue for later mesh processing
            if (i > -meshLoadRadius && i < meshLoadRadius && j > -meshLoadRadius && j < meshLoadRadius) {
                if (loadedChunkmeshes.find(chunkPos) == loadedChunkmeshes.end()) {
                    chunkQueue.push(chunkPos);
                    loadedChunkmeshes.insert(chunkPos);
                }
            }

            // Skip if the chunk data is already loaded
            if (chunks.find(chunkPos) != chunks.end()) {
                continue;
            }

            // Populate chunk data and add it to the chunks map
            auto blocks = chunkTemplate.populateChunk(chunkX, chunkZ);
            chunks[chunkPos] = blocks;
        }
    }

    // Generate mesh data only for chunks that need a mesh
    while (!chunkQueue.empty()) {
        std::pair<int, int> chunkPos = chunkQueue.front();
        chunkQueue.pop();


        // Ensure the chunk data exists before generating the mesh
        if (chunks.find(chunkPos) == chunks.end()) {
            continue;
        }

        auto blocks = chunks[chunkPos];

        // Generate mesh data
        auto meshData = chunkmaker.createMeshDataFromChunk(chunkPos.first, chunkPos.second, blocks, chunks);

        // Push the generated mesh data to the meshQueue
        meshQueue.push({chunkPos, meshData});
    }
}

void World::processChunks(Camera& camera) {
    float worldOffsetX = (WORLD_X_DIM * CHUNK_X_DIM) / 2.0f;
    float worldOffsetZ = (WORLD_Z_DIM * CHUNK_Z_DIM) / 2.0f;

    int centerX = (static_cast<int>(camera.Position.x + worldOffsetX) / CHUNK_X_DIM);
    int centerZ = (static_cast<int>(camera.Position.z + worldOffsetZ) / CHUNK_Z_DIM);
    
    // Process mesh data and add it to the world
    while (!meshQueue.empty()) {
        std::pair<std::vector<Vertex>, std::vector<GLuint>> meshData = meshQueue.front().second;
        std::pair<int, int> chunkPos = meshQueue.front().first;
        meshQueue.pop();

        addChunkMeshToWorld(chunkPos.first, chunkPos.second, meshData.first, meshData.second);
    }

    // Remove chunks out of view
    std::vector<std::pair<int, int>> chunksToDelete;

    // Find chunks to delete (out of load radius)
    for (auto& [pos, _] : chunks) {
        int xDist = abs(pos.first - centerX);
        int zDist = abs(pos.second - centerZ);
        if (xDist > dataLoadRadius || zDist > dataLoadRadius) {
            chunksToDelete.push_back(pos);
        }
    }

    // Remove chunks that are too far from the player
    for (auto& pos : chunksToDelete) {
        removeChunkFromChunks(pos.first, pos.second);
    }

    chunksToDelete.clear();

    // Find chunks to delete (out of load radius)
    for (auto& [pos, _] : chunks) {
        int xDist = abs(pos.first - centerX);
        int zDist = abs(pos.second - centerZ);
        if (xDist > meshLoadRadius || zDist > meshLoadRadius) {
            loadedChunkmeshes.erase(pos);
            chunksToDelete.push_back(pos);
        }
    }

    // Remove chunks that are too far from the player
    for (auto& pos : chunksToDelete) {
        removeChunkFromWorld(pos.first, pos.second);
    }
}

void World::removeChunkFromWorld(int xPos, int zPos) {
    std::pair<int, int> pos = {xPos, zPos};
    if (chunkVAOs.find(pos) != chunkVAOs.end()) {
        glDeleteVertexArrays(1, &chunkVAOs[pos]);
        glDeleteBuffers(1, &chunkVBOs[pos]);
        glDeleteBuffers(1, &chunkEBOs[pos]);

        chunkVAOs.erase(pos);
        chunkVBOs.erase(pos);
        chunkEBOs.erase(pos);
        chunkIndexCounts.erase(pos);
    }
}

void World::removeChunkFromChunks(int xPos, int zPos) {
    std::pair<int, int> pos = {xPos, zPos};
    chunks.erase(pos);
}

void World::addChunkMeshToWorld(int xPos, int zPos, std::vector<Vertex> chunkVertices, std::vector<GLuint> chunkIndices) {
    GLuint vao, vbo, ebo;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, chunkVertices.size() * sizeof(Vertex), chunkVertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, chunkIndices.size() * sizeof(GLuint), chunkIndices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec3));
    glEnableVertexAttribArray(1);
    glVertexAttribIPointer(2, 1, GL_BYTE, sizeof(Vertex), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    std::pair<int, int> chunkPos = {xPos, zPos};
    chunkVAOs[chunkPos] = vao;
    chunkVBOs[chunkPos] = vbo;
    chunkEBOs[chunkPos] = ebo;
    chunkIndexCounts[chunkPos] = chunkIndices.size();
}

void World::textureActivate() {
    atlas->texUnit(*shader, "tex0", 0);
    atlas->Bind();
}

// In World::Render - Fix Chunk Position Offsets
void World::Render(Camera& camera) {
    processChunks(camera);

    shader->Activate();
    camera.Inputs();
    camera.updateMatrix();
    camera.Matrix(*shader, "camMatrix");

    frustum.update(camera);
    textureActivate();

    float worldOffsetX = (WORLD_X_DIM * CHUNK_X_DIM) / 2.0f;
    float worldOffsetZ = (WORLD_Z_DIM * CHUNK_Z_DIM) / 2.0f;

    for (auto& [pos, vao] : chunkVAOs) {
        int xPos = pos.first;
        int zPos = pos.second;

        // Adjust chunk position offsets to be relative to the world center
        glm::vec3 min = {
            (float)(xPos * CHUNK_X_DIM) - worldOffsetX,
            0.0f,
            (float)(zPos * CHUNK_Z_DIM) - worldOffsetZ
        };

        glm::vec3 max = {
            (float)((xPos + 1) * CHUNK_X_DIM) - worldOffsetX,
            CHUNK_Y_DIM,
            (float)((zPos + 1) * CHUNK_Z_DIM) - worldOffsetZ
        };


        if (frustum.intersectsAABB(min, max)) {
            glBindVertexArray(vao);
            glDrawElements(GL_TRIANGLES, chunkIndexCounts[pos], GL_UNSIGNED_INT, 0);
        }
    }

    glBindVertexArray(0);
}

