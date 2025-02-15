#ifndef CHUNKMESH_H
#define CHUNKMESH_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <vector>
#include <iostream>
#include <array>

#include "constants.h"

struct Direction {
    int xOffset, yOffset, zOffset;
};

struct chunkMesh {
    std::vector<GLfloat> cubeVertices = { 
        // Each face has 4 vertices * 3 coords (x, y, z) = 12 per face
        -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  -0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f,   0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,   0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f, -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,   0.5f, -0.5f, -0.5f
    };

    std::vector<GLfloat> blockTexCoords = { 
        0.0f, 0.0f,  1.0f, 0.0f,  1.0f, 1.0f,  0.0f, 1.0f,
        1.0f, 0.0f,  0.0f, 0.0f,  0.0f, 1.0f,  1.0f, 1.0f,
        0.0f, 0.0f,  1.0f, 0.0f,  1.0f, 1.0f,  0.0f, 1.0f,
        1.0f, 0.0f,  0.0f, 0.0f,  0.0f, 1.0f,  1.0f, 1.0f,
        0.0f, 0.0f,  0.0f, 1.0f,  1.0f, 1.0f,  1.0f, 0.0f,
        1.0f, 1.0f,  1.0f, 0.0f,  0.0f, 0.0f,  0.0f, 1.0f
    };

    std::vector<GLuint> cubeIndices = { 
        0, 1, 2,  2, 3, 0,  4, 5, 6,  6, 7, 4,
        8, 9, 10,  10, 11, 8,  12, 13, 14,  14, 15, 12,
        16, 17, 18,  18, 19, 16,  20, 21, 22,  22, 23, 20
    };
    std::pair<std::vector<GLfloat>, std::vector<GLuint>> createMeshDataFromChunk(std::vector<GLuint> blocksByPosition);
};



#endif
