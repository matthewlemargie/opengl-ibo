#include "chunkMesh.h"

std::pair<std::vector<GLfloat>, std::vector<GLuint>> chunkMesh::createMeshDataFromChunk(std::vector<GLuint> blocksByPosition) {
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;

    std::array<Direction, 6> dirs = {
        Direction{0, 0, 1}, 
        Direction{0, 0, -1},
        Direction{1, 0, 0},
        Direction{-1, 0, 0},
        Direction{0, 1, 0},
        Direction{0, -1, 0},
    };

    for (int z = 0; z < CHUNK_Z_DIM; ++z) {
        for (int y = 0; y < CHUNK_Y_DIM; ++y) {
            for (int x = 0; x < CHUNK_X_DIM; ++x) {
                int i = x + (y * CHUNK_X_DIM) + (z * CHUNK_X_DIM * CHUNK_Y_DIM);  // Corrected flat index calculation

                if (blocksByPosition[i] == AIR) continue;

                for (int face = 0; face < 6; ++face) {
                    int nx = x + dirs[face].xOffset;
                    int ny = y + dirs[face].yOffset;
                    int nz = z + dirs[face].zOffset;

                    bool isOutOfBounds = nx < 0 || ny < 0 || nz < 0 || nx >= CHUNK_X_DIM || ny >= CHUNK_Y_DIM || nz >= CHUNK_Z_DIM;
                    int neighborIndex = nx + (ny * CHUNK_X_DIM) + (nz * CHUNK_X_DIM * CHUNK_Y_DIM);

                    if (isOutOfBounds || blocksByPosition[neighborIndex] == AIR) {
                        int vertexOffset = vertices.size() / 5; // Each vertex has (x, y, z, u, v)

                        for (int j = 0; j < 4; ++j) {
                            vertices.push_back(cubeVertices[face * 12 + j * 3] + x);
                            vertices.push_back(cubeVertices[face * 12 + j * 3 + 1] + y);
                            vertices.push_back(cubeVertices[face * 12 + j * 3 + 2] + z);
                            vertices.push_back(blockTexCoords[face * 8 + j * 2]);
                            vertices.push_back(blockTexCoords[face * 8 + j * 2 + 1]);
                        }

                        indices.push_back(vertexOffset + 0);
                        indices.push_back(vertexOffset + 1);
                        indices.push_back(vertexOffset + 2);
                        indices.push_back(vertexOffset + 2);
                        indices.push_back(vertexOffset + 3);
                        indices.push_back(vertexOffset + 0);
                    }
                }
            }
        }
    }
    return std::make_pair(vertices, indices);
}
