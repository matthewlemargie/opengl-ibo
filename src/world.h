#ifndef WORLD_H
#define WORLD_H
#define GLM_ENABLE_EXPERIMENTAL

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <unordered_map>
#include <array>
#include <iterator>
#include <chrono>

#include "OpenGL/VAO.h"
#include "OpenGL/VBO.h"
#include "OpenGL/EBO.h"
#include "OpenGL/IBO.h"
#include "OpenGL/Texture.h"
#include "OpenGL/shaderClass.h"
#include "OpenGL/Camera.h"
#include "OpenGL/skybox.h"
#include "AABB.h"

constexpr int CHUNK_X_DIM = 16;
constexpr int CHUNK_Y_DIM = 16;
constexpr int CHUNK_Z_DIM = 16;
constexpr int CHUNK_TOTAL_BLOCKS = CHUNK_X_DIM * CHUNK_Y_DIM * CHUNK_Z_DIM;

enum BlockID {
    AIR = -1,
    GRASS,
    BLOCKID_COUNT,
};

struct World {
    GLfloat cubeVertices[192] = {
        // Position           // Normal         // UV
        // Front face
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,  // Bottom-left
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,  // Bottom-right
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,  // Top-right
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,  // Top-left

        // Back face
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,  // Bottom-left
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,  // Bottom-right
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,  // Top-right
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,  // Top-left

        // Left face
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,  // Bottom-left
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,  // Bottom-right
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,  // Top-right
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,  // Top-left

        // Right face
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,  // Bottom-left
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,  // Bottom-right
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,  // Top-right
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,  // Top-left

        // Top face
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,  // Bottom-left
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,  // Bottom-right
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,  // Top-right
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,  // Top-left

        // Bottom face
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,  // Bottom-left
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,  // Bottom-right
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,  // Top-right
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f   // Top-left
    };

    GLfloat blockTexCoords[48] = {
        // grass block texture
        // Front face
        0.0f, 0.0f,
        1.0f, 0.0f, 
        1.0f, 1.0f, 
        0.0f, 1.0f,
        // Back face
        1.0f, 0.0f,
        0.0f, 0.0f, 
        0.0f, 1.0f, 
        1.0f, 1.0f,
        // Left face
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        // Right face
        1.0f, 0.0f, 
        0.0f, 0.0f, 
        0.0f, 1.0f, 
        1.0f, 1.0f, 
        // Top face
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f, 
        1.0f, 0.0f, 
        // Bottom face
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f, 
        0.0f, 1.0f  
    };

    GLuint cubeIndices[36] = {
        // Front face
        0, 1, 2,  2, 3, 0,
        // Back face
        4, 5, 6,  6, 7, 4,
        // Left face
        8, 9, 10,  10, 11, 8,
        // Right face
        12, 13, 14,  14, 15, 12,
        // Top face
        16, 17, 18,  18, 19, 16,
        // Bottom face
        20, 21, 22,  22, 23, 20
    };

    bool firstChunk = true;

    Skybox skybox;
    // VAO vao;
    // VBO vbo;
    // EBO ebo;
    // IBO ibo;
    std::vector<GLuint> vaos;
    std::vector<GLuint> vbos;
    std::vector<GLuint> ebos;
    std::vector<GLuint> ibos;

    unordered_map<GLuint, std::array<GLuint, 4>> blocktypeToBuffersIDsMap;

    Texture* grass;
    Shader* shader;

    AABB modelAABB;

    World(GLContext* context);
    ~World();

    void textureActivate();
    void addChunkToWorld(std::vector<std::vector<GLfloat>> newBlockInstances);
    void addInstance(int blockID, std::array<int,3> posInWorld);
    void Render(Camera& camera);
    std::vector<std::vector<GLfloat>> populateChunk();
};
#endif
