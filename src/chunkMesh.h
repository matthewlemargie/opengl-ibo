#ifndef CHUNKMESH_H
#define CHUNKMESH_H

#include <glm/glm.hpp>
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
        // North
        1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        // South
        0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        // West
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f,
        // East
        1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 0.0f,
        // Bottom
        1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        // Top
        0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f
    };

    std::vector<GLfloat> blockTexCoords = { 
        // North
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        // South
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        // West
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        // East
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        // Bottom
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        // Top
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
    };

    std::vector<GLuint> cubeIndices = { 
        0, 3, 1,  0, 2, 3,  // North
        4, 7, 5,  4, 6, 7,  // South
        8, 11, 9, 8, 10, 11, // West
        12, 15, 13, 12, 14, 15, // East
        16, 19, 17, 16, 18, 19, // Bottom
        20, 23, 21, 20, 22, 23  // Top
    };
    std::pair<std::vector<BlockVertex>, std::vector<GLuint>> createMeshDataFromChunk(int xPos, int zPos, std::vector<GLuint> blocksByPosition);
};



#endif
