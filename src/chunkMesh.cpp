#include "chunkMesh.h"

std::pair<std::vector<GLfloat>, std::vector<GLuint>> chunkMesh::createMeshDataFromChunk(std::vector<GLuint> blocksByPosition) {
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;

    std::array<Direction, 6> dirs = {
        Direction{0, 1, 0},  // Top
        Direction{0, -1, 0}, // Bottom
        Direction{1, 0, 0},  // Right
        Direction{-1, 0, 0}, // Left
        Direction{0, 0, 1},  // Front
        Direction{0, 0, -1}  // Back
    };

    // Loop through each block in the chunk
    for (int z = 0; z < CHUNK_Z_DIM; ++z) {
        for (int y = 0; y < CHUNK_Y_DIM; ++y) {
            for (int x = 0; x < CHUNK_X_DIM; ++x) {
                int i = x + (y * CHUNK_X_DIM) + (z * CHUNK_X_DIM * CHUNK_Y_DIM);  // Corrected flat index calculation

                // Skip air blocks
                if (blocksByPosition[i] == AIR) continue;

                // Loop through all 6 faces of a block
                for (int face = 0; face < 6; ++face) {
                    int nx = x + dirs[face].xOffset;
                    int ny = y + dirs[face].yOffset;
                    int nz = z + dirs[face].zOffset;

                    bool isEdge = (nx < 0 || nx >= CHUNK_X_DIM || ny < 0 || ny >= CHUNK_Y_DIM || nz < 0 || nz >= CHUNK_Z_DIM);

                    // Skip if adjacent block is not air or if it’s within bounds and not air
                    if (!isEdge) {
                        int neighborIndex = nx + (ny * CHUNK_X_DIM) + (nz * CHUNK_X_DIM * CHUNK_Y_DIM);
                        if (blocksByPosition[neighborIndex] != AIR) continue;
                    }

                    // Calculate the vertex offset for the block face
                    int vertexOffset = vertices.size() / 5; // Each vertex has (x, y, z, u, v)

                    // Add 4 vertices for this block face
                    for (int j = 0; j < 4; ++j) {
                        vertices.push_back(cubeVertices[face * 12 + j * 3] + x);          // x position
                        vertices.push_back(cubeVertices[face * 12 + j * 3 + 1] + y);      // y position
                        vertices.push_back(cubeVertices[face * 12 + j * 3 + 2] + z);      // z position
                        vertices.push_back(blockTexCoords[face * 8 + j * 2]);             // u
                        vertices.push_back(blockTexCoords[face * 8 + j * 2 + 1]);         // v
                    }

                    // Add indices for the two triangles (6 vertices) of the block face
                    indices.push_back(vertexOffset + 0);
                    indices.push_back(vertexOffset + 1);
                    indices.push_back(vertexOffset + 2);
                    indices.push_back(vertexOffset + 0);
                    indices.push_back(vertexOffset + 2);
                    indices.push_back(vertexOffset + 3);
                }
            }
        }
    }

    return {vertices, indices}; // Return both vertices and indices as a pair
}

