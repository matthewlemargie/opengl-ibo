#ifndef BLOCKS_H
#define BLOCKS_H
#define GLM_ENABLE_EXPERIMENTAL

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <unordered_map>
#include <array>

#include "OpenGL/VAO.h"
#include "OpenGL/VBO.h"
#include "OpenGL/EBO.h"
#include "OpenGL/IBO.h"
#include "OpenGL/Texture.h"
#include "OpenGL/shaderClass.h"
#include "OpenGL/Camera.h"
#include "AABB.h"

struct Blocks {
    GLfloat cubeVertices[192] = {
        // Position           // Normal         // UV
        // Front face
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   // Bottom-left
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   // Bottom-right
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   // Top-right
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   // Top-left

        // Back face
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   // Bottom-left
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   // Bottom-right
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   // Top-right
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   // Top-left

        // Left face
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,   // Bottom-left
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,   // Bottom-right
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,   // Top-right
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,   // Top-left

        // Right face
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,   // Bottom-left
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,   // Bottom-right
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,   // Top-right
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,   // Top-left

        // Top face
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,   // Bottom-left
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,   // Bottom-right
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,   // Top-right
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,   // Top-left

        // Bottom face
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,   // Bottom-left
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,   // Bottom-right
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,   // Top-right
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,   // Top-left
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

    VAO vao;
    VBO vbo;
    IBO ibo;
    std::vector<GLuint> ibos;
    std::vector<GLuint> texvbos;

    unordered_map<unsigned int, std::array<GLuint, 2>> blocktypeToBufferIDsMap;

    Texture* grass;
    Shader* shader;

    float* scale;
    glm::vec4* lightColor;
    glm::vec3* lightPos;

    AABB modelAABB;
    int xDim, yDim, zDim, totalBlocks, totalBlockIDs;

    Blocks(int xDim, int yDim, int zDim, int totalBlocks, int totalBlockIDs);
    ~Blocks();

    void textureActivate(Camera& camera);
    void addInstance(int blockID, std::array<int,3> posInWorld);
    void Draw();
};

#endif
