#include "world.h"

World::World(GLContext* context) 
{
    auto start = std::chrono::high_resolution_clock::now();

    shader = new Shader("shaders/block_vert.glsl", "shaders/block_frag.glsl");
    atlas = new Texture("assets/textures/grass.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);

    float totalVRAMUsage = 0.0f;

    Chunk chunkTemplate;
    // Generate chunks and store them as individual VAOs, VBOs, and chunkEBOs
    for (int i = 0; i < WORLD_X_DIM; ++i) {
        for (int j = 0; j < WORLD_Z_DIM; ++j) {
            auto blocks = chunkTemplate.populateChunk(i, j);
            chunks[{i, j}] = blocks;
        }
    }

    std::vector<std::thread> threads;

    std::vector<std::pair<std::vector<Vertex>, std::vector<GLuint>>> meshDatas(WORLD_X_DIM * WORLD_Z_DIM);
    std::mutex meshDatasMutex;

    int numThreads = 8;
    int counter = 0;

    for (int i = 0; i < WORLD_X_DIM; ++i) {
        for (int j = 0; j < WORLD_Z_DIM; ++j) {
            threads.emplace_back([=, &meshDatasMutex, &meshDatas]() {  // Capture mutex and vector by reference
                auto blocks = chunks[{i, j}];
                auto meshData = chunkmaker.createMeshDataFromChunk(i, j, blocks, chunks);
                
                // ✅ Lock mutex safely
                std::lock_guard<std::mutex> lock(meshDatasMutex);
                meshDatas[i * WORLD_X_DIM + j] = meshData;
            });
            if (counter == numThreads) {
                for (auto& thread : threads) {
                    thread.join();
                }
                threads.clear();
                counter = 0;
            }
            else
                counter++;
        }
    }

    for (auto& thread : threads) {
        thread.join();
    }

    for (size_t i = 0; i < meshDatas.size(); ++i) {
        int z = i % WORLD_X_DIM;
        int x = i / WORLD_X_DIM;
        auto& vertices = std::get<0>(meshDatas[i]); 
        auto& indices = std::get<1>(meshDatas[i]); 
        addChunkMeshToWorld(x, z, vertices, indices);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "World initialized in " << duration.count() << "s" << std::endl;
    std::cout << "Total space used on VRAM: " << totalVRAMUsage / 1000000.0f << "MBs" << std::endl;
}

World::~World() {
    chunks.clear();

    delete atlas;
    delete shader;
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

    // Store VAO, VBO, and EBO in maps
    std::pair<int, int> chunkPos = {xPos, zPos};
    chunkVAOs[chunkPos] = vao;
    chunkVBOs[chunkPos] = vbo;
    chunkEBOs[chunkPos] = ebo;
    chunkIndexCounts[chunkPos] = chunkIndices.size();
}

void World::addBlock(int blockID, int posInChunk, std::array<int, 2> posInWorld) {
}

void World::textureActivate() {
    atlas->texUnit(*shader, "tex0", 0);
    atlas->Bind();
}

void World::Render(Camera& camera) {
    // frustum culling

    auto start = std::chrono::high_resolution_clock::now();
    shader->Activate();
    camera.Inputs();
    camera.updateMatrix();
    camera.Matrix(*shader, "camMatrix");

    frustum.update(camera);
    textureActivate();
    for (auto& [pos, vao] : chunkVAOs) {
        // Offsets to center the world
        float worldOffsetX = (WORLD_X_DIM * CHUNK_X_DIM) / 2.0f;
        float worldOffsetZ = (WORLD_Z_DIM * CHUNK_Z_DIM) / 2.0f;

        // Assume pos is stored as (x, z)
        int xPos = std::get<0>(pos);
        int zPos = std::get<1>(pos);

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
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    // std::cout << "Entire render call took " << duration.count() << " s" << std::endl;
}

