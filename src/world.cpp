#include "world.h"

World::World(GLContext* context) 
{
    auto start = std::chrono::high_resolution_clock::now();
    shader = new Shader("shaders/block_vert.glsl", "shaders/block_frag.glsl");
    atlas = new Texture("assets/textures/grass.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);

    float vboBufferSize = 10000 * TOTAL_BLOCKS_RENDERED * (3 * sizeof(GLfloat));
    float eboBufferSize = 10000 * TOTAL_BLOCKS_RENDERED * sizeof(GLuint);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vboBufferSize, nullptr, GL_STATIC_DRAW);
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, eboBufferSize, nullptr, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    cout << "world initialized in " << duration.count() << "s" << endl;

    float totalVRAMUsage = vboBufferSize + eboBufferSize;
    cout << "Total space used on VRAM: " << totalVRAMUsage / 1000000.0f << "MBs" << endl;
}

World::~World() {
    delete atlas;
    delete shader;
}

void World::addChunkMeshToWorld(std::vector<GLfloat> chunkVertices, std::vector<GLuint> chunkIndices) {
    // posInWorld needed to know where in vbo/ebo to place vertices, indices
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, chunkVertices.size(), chunkVertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, chunkIndices.size(), chunkIndices.data(), GL_STATIC_DRAW);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 16 * 16 * 6 * 6, GL_UNSIGNED_INT, 0);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    // std::cout << "Entire render call took " << duration.count() << " s" << std::endl;
}

