#include "world.h"

World::World(GLContext* context) 
{
    auto start = std::chrono::high_resolution_clock::now();

    shader = new Shader("shaders/block_vert.glsl", "shaders/block_frag.glsl");
    atlas = new Texture("assets/textures/grass.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);

    float totalVRAMUsage = 0.0f;

    // Generate chunks and store them as individual VAOs, VBOs, and EBOs
    std::vector<GLuint> chunk;
    for (int i = 0; i < WORLD_X_DIM; ++i) {
        for (int j = 0; j < WORLD_Z_DIM; ++j) {
            chunk = chunkmaker.populateChunk(i, j);
            auto meshData = ChunkMesh.createMeshDataFromChunk(i, j, chunk);
            auto vertices = std::get<0>(meshData); 
            auto indices = std::get<1>(meshData); 

            addChunkMeshToWorld(i, j, vertices, indices);

            // Accumulate VRAM usage
            totalVRAMUsage += vertices.size() * sizeof(GLfloat);
            totalVRAMUsage += indices.size() * sizeof(GLuint);
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "World initialized in " << duration.count() << "s" << std::endl;
    std::cout << "Total space used on VRAM: " << totalVRAMUsage / 1000000.0f << "MBs" << std::endl;
}

World::~World() {
    delete atlas;
    delete shader;
}

void World::addChunkMeshToWorld(int xPos, int zPos, std::vector<BlockVertex> chunkVertices, std::vector<GLuint> chunkIndices) {
    GLuint vao, vbo, ebo;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, chunkVertices.size() * sizeof(BlockVertex), chunkVertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, chunkIndices.size() * sizeof(GLuint), chunkIndices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(BlockVertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(BlockVertex), (void*)sizeof(glm::vec3));
    glEnableVertexAttribArray(1);
    glVertexAttribIPointer(2, 1, GL_BYTE, sizeof(BlockVertex), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
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
    auto start = std::chrono::high_resolution_clock::now();
    camera.Inputs();
    camera.updateMatrix();
    camera.Matrix(*shader, "camMatrix");

    textureActivate();
    for (auto& [pos, vao] : chunkVAOs) {
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, chunkIndexCounts[pos], GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(0);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    // std::cout << "Entire render call took " << duration.count() << " s" << std::endl;
}

