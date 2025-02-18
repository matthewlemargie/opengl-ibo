#include "world.h"

World::World(GLContext* context) 
{
    auto start = std::chrono::high_resolution_clock::now();
    shader = new Shader("shaders/block_vert.glsl", "shaders/block_frag.glsl");
    atlas = new Texture("assets/textures/grass.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);

    float vboBufferSize = TOTAL_BLOCKS_RENDERED * (5 * 4 * 6 * sizeof(GLfloat));
    float eboBufferSize = TOTAL_BLOCKS_RENDERED * (6 * 6 * sizeof(GLuint));

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

    currentVBOOffset = 0;
    currentEBOOffset = 0;

    std::vector<GLuint> chunk;
    for (int i = 0; i < WORLD_X_DIM; ++i) {
        for (int j = 0; j < WORLD_Z_DIM; ++j) {
            chunk = chunkmaker.populateChunk(i, j);
            auto meshData = ChunkMesh.createMeshDataFromChunk(i, j, chunk);
            auto vertices = std::get<0>(meshData); 
            auto indices = std::get<1>(meshData); 
            addChunkMeshToWorld(i, j, vertices, indices);
        }
    }

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

void World::addChunkMeshToWorld(int xPos, int zPos, std::vector<GLfloat> chunkVertices, std::vector<GLuint> chunkIndices) {
    if (chunkVertices.empty() || chunkIndices.empty()) return;  // Ignore empty chunks

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Store the offset where this chunk's vertices start
    size_t vboOffsetBytes = currentVBOOffset * sizeof(GLfloat);
    glBufferSubData(GL_ARRAY_BUFFER, vboOffsetBytes, chunkVertices.size() * sizeof(GLfloat), chunkVertices.data());

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    
    // Store index offset before inserting new indices
    size_t eboOffsetBytes = currentEBOOffset * sizeof(GLuint);
    
    // Adjust indices to reference the correct vertices
    size_t baseVertexIndex = currentVBOOffset / 5;  // 5 floats per vertex
    for (size_t i = 0; i < chunkIndices.size(); i++) {
        chunkIndices[i] += baseVertexIndex;  // Offset indices to point to the correct vertices
    }
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, eboOffsetBytes, chunkIndices.size() * sizeof(GLuint), chunkIndices.data());

    // Store the index buffer offset for this chunk
    chunkIndexOffsets[std::make_pair(xPos, zPos)] = currentEBOOffset;

    // Update offsets for the next chunk
    currentVBOOffset += chunkVertices.size();
    currentEBOOffset += chunkIndices.size();

    glBindVertexArray(0);
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

    // Convert chunk data into arrays for glMultiDrawElements
    std::vector<GLsizei> indexCounts;
    std::vector<void*> indexOffsets;

    for (const auto& [pos, indexOffset] : chunkIndexOffsets) {
        indexCounts.push_back(6 * 6 * CHUNK_TOTAL_BLOCKS);  // Indices per chunk
        indexOffsets.push_back(reinterpret_cast<void*>(indexOffset * sizeof(GLuint)));
    }

    // Use glMultiDrawElements to batch draw all chunks
    glMultiDrawElements(GL_TRIANGLES, indexCounts.data(), GL_UNSIGNED_INT, indexOffsets.data(), indexCounts.size());

    glBindVertexArray(0);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    // std::cout << "Entire render call took " << duration.count() << " s" << std::endl;
}

