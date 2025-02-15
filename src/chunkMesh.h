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
        // Face 0: Front (z = +0.5)
        -0.5f, -0.5f,  0.5f,   0.5f, -0.5f,  0.5f,   0.5f,  0.5f,  0.5f,   -0.5f,  0.5f,  0.5f,
        // Face 1: Back (z = -0.5)
        -0.5f, -0.5f, -0.5f,   0.5f, -0.5f, -0.5f,   0.5f,  0.5f, -0.5f,   -0.5f,  0.5f, -0.5f,
        // Face 2: Right (x = +0.5)
        0.5f, -0.5f, -0.5f,   0.5f, -0.5f,  0.5f,   0.5f,  0.5f,  0.5f,    0.5f,  0.5f, -0.5f,
        // Face 3: Left (x = -0.5)
        -0.5f, -0.5f, -0.5f,  -0.5f, -0.5f,  0.5f,  -0.5f,  0.5f,  0.5f,   -0.5f,  0.5f, -0.5f,
        // Face 4: Top (y = +0.5)
        -0.5f,  0.5f, -0.5f,  -0.5f,  0.5f,  0.5f,   0.5f,  0.5f,  0.5f,    0.5f,  0.5f, -0.5f,
        // Face 5: Bottom (y = -0.5)
        -0.5f, -0.5f, -0.5f,  -0.5f, -0.5f,  0.5f,   0.5f, -0.5f,  0.5f,    0.5f, -0.5f, -0.5f
    };

    std::vector<GLfloat> blockTexCoords = { 
        // Face 0
        0.0f, 0.0f,  1.0f, 0.0f,  1.0f, 1.0f,  0.0f, 1.0f,
        // Face 1
        1.0f, 0.0f,  0.0f, 0.0f,  0.0f, 1.0f,  1.0f, 1.0f,
        // Face 2
        0.0f, 0.0f,  1.0f, 0.0f,  1.0f, 1.0f,  0.0f, 1.0f,
        // Face 3
        1.0f, 0.0f,  0.0f, 0.0f,  0.0f, 1.0f,  1.0f, 1.0f,
        // Face 4
        0.0f, 0.0f,  0.0f, 1.0f,  1.0f, 1.0f,  1.0f, 0.0f,
        // Face 5
        1.0f, 1.0f,  1.0f, 0.0f,  0.0f, 0.0f,  0.0f, 1.0f
    };

    std::vector<GLuint> cubeIndices = { 
        0, 1, 2,  2, 3, 0,  // Face 0
        4, 5, 6,  6, 7, 4,  // Face 1
        8, 9, 10, 10, 11, 8, // Face 2
        12, 13, 14, 14, 15, 12, // Face 3
        16, 17, 18, 18, 19, 16, // Face 4
        20, 21, 22, 22, 23, 20  // Face 5
    };
    std::pair<std::vector<GLfloat>, std::vector<GLuint>> createMeshDataFromChunk(std::pair<int, int> posInWorld, std::vector<GLuint> blocksByPosition);
};



#endif
