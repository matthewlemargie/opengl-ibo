#include "chunkMesh.h"

std::pair<std::vector<Vertex>, std::vector<GLuint>> chunkMesh::createMeshDataFromChunk(int xPos, int zPos, std::vector<GLuint> blocksByPosition) {
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    std::array<Direction, 6> dirs = {
        Direction{0, 0, -1},
        Direction{0, 0, 1}, 
        Direction{-1, 0, 0},
        Direction{1, 0, 0},
        Direction{0, -1, 0},
        Direction{0, 1, 0},
    };


    for (int x = 0; x < CHUNK_X_DIM; ++x) {
        for (int z = 0; z < CHUNK_Z_DIM; ++z) {
            for (int y = 0; y < CHUNK_Y_DIM; ++y) {
                int i = x + (y * CHUNK_X_DIM) + (z * CHUNK_X_DIM * CHUNK_Y_DIM);

                if (blocksByPosition[i] == AIR) continue;

                for (int face = 0; face < 6; ++face) {
                    int nx = x + dirs[face].xOffset;
                    int ny = y + dirs[face].yOffset;
                    int nz = z + dirs[face].zOffset;

                    if (ny < 1) continue;
                    bool isOutOfBounds = nx < 0 || ny < 0 || nz < 0 || nx >= CHUNK_X_DIM || ny >= CHUNK_Y_DIM || nz >= CHUNK_Z_DIM;
                    int neighborIndex = nx + (ny * CHUNK_X_DIM) + (nz * CHUNK_X_DIM * CHUNK_Y_DIM);

                    if (isOutOfBounds || blocksByPosition[neighborIndex] == AIR) {
                        int vertexOffset = vertices.size();

                        for (int j = 0; j < 4; ++j) {
                            Vertex vertex;
                            vertex.position = glm::vec3(cubeVertices[face * 12 + j * 3] + x + (CHUNK_X_DIM * xPos) - ((float)CHUNK_X_DIM * WORLD_X_DIM / 2), cubeVertices[face * 12 + j * 3 + 1] + y, cubeVertices[face * 12 + j * 3 + 2] + z + (CHUNK_Z_DIM * zPos) - ((float)CHUNK_Z_DIM * WORLD_X_DIM / 2));
                            vertex.texCoord = glm::vec2(blockTexCoords[face * 8 + j * 2], blockTexCoords[face * 8 + j * 2 + 1]);
                            vertex.direction = face;
                            vertices.push_back(vertex);
                        }

                        indices.push_back(vertexOffset + 0);
                        indices.push_back(vertexOffset + 3);
                        indices.push_back(vertexOffset + 1);
                        indices.push_back(vertexOffset + 0);
                        indices.push_back(vertexOffset + 2);
                        indices.push_back(vertexOffset + 3);
                    }
                }
            }
        }
    }
    return std::make_pair(vertices, indices);
}
