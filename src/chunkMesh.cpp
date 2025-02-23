#include "chunkMesh.h"

int chunkMesh::oppositeFace(int face) {
    static constexpr int opposite[6] = {1, 0, 3, 2, 5, 4}; 
    return opposite[face];
}

std::pair<std::vector<Vertex>, std::vector<GLuint>> chunkMesh::createMeshDataFromChunk(
    int xPos, int zPos, 
    const std::vector<int>& blocksByPosition,
    std::unordered_map<std::pair<int, int>, std::vector<int>, pair_hash>& chunks)
{
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    // Face visibility mask (each block has 6 bits to indicate if a face is visible)
    std::vector<uint8_t> faceVisibility(CHUNK_X_DIM * CHUNK_Y_DIM * CHUNK_Z_DIM, 0x3F); // 0b111111 (all faces visible initially)

    // Step 1: Precompute face occlusion within the same chunk
    for (int x = 0; x < CHUNK_X_DIM; ++x) {
        for (int z = 0; z < CHUNK_Z_DIM; ++z) {
            for (int y = 0; y < CHUNK_Y_DIM; ++y) {
                int i = x + (y * CHUNK_X_DIM) + (z * CHUNK_X_DIM * CHUNK_Y_DIM);

                if (blocksByPosition[i] == AIR) continue;

                for (int face = 0; face < 6; ++face) {
                    int nx = x + dirs[face].xOffset;
                    int ny = y + dirs[face].yOffset;
                    int nz = z + dirs[face].zOffset;

                    if (ny < 0 || ny >= CHUNK_Y_DIM) continue;

                    bool isOutOfBounds = nx < 0 || nz < 0 || nx >= CHUNK_X_DIM || nz >= CHUNK_Z_DIM;
                    int neighborIndex = nx + (ny * CHUNK_X_DIM) + (nz * CHUNK_X_DIM * CHUNK_Y_DIM);

                    if (!isOutOfBounds && blocksByPosition[neighborIndex] >= 0) {
                        // If neighbor is solid, hide this face
                        faceVisibility[i] &= ~(1 << face);
                        faceVisibility[neighborIndex] &= ~(1 << oppositeFace(face));
                    }
                }
            }
        }
    }

    // Step 2: Process only visible faces and handle chunk borders
    for (int x = 0; x < CHUNK_X_DIM; ++x) {
        for (int z = 0; z < CHUNK_Z_DIM; ++z) {
            for (int y = 0; y < CHUNK_Y_DIM; ++y) {
                int i = x + (y * CHUNK_X_DIM) + (z * CHUNK_X_DIM * CHUNK_Y_DIM);
                if (blocksByPosition[i] == AIR) continue;

                for (int face = 0; face < 6; ++face) {
                    if (!(faceVisibility[i] & (1 << face))) continue; // Skip hidden faces

                    int nx = x + dirs[face].xOffset;
                    int ny = y + dirs[face].yOffset;
                    int nz = z + dirs[face].zOffset;

                    bool shouldRenderFace = false;
                    if (nx < 0 || nz < 0 || nx >= CHUNK_X_DIM || nz >= CHUNK_Z_DIM) {
                        // Neighbor chunk check
                        int neighborChunkX = xPos;
                        int neighborChunkZ = zPos;
                        int neighborLocalX = nx;
                        int neighborLocalZ = nz;

                        if (nx < 0) { neighborChunkX -= 1; neighborLocalX = CHUNK_X_DIM - 1; }
                        else if (nx >= CHUNK_X_DIM) { neighborChunkX += 1; neighborLocalX = 0; }

                        if (nz < 0) { neighborChunkZ -= 1; neighborLocalZ = CHUNK_Z_DIM - 1; }
                        else if (nz >= CHUNK_Z_DIM) { neighborChunkZ += 1; neighborLocalZ = 0; }

                        auto it = chunks.find({neighborChunkX, neighborChunkZ});
                        if (it != chunks.end() && ny >= 0 && ny < CHUNK_Y_DIM) {
                            std::vector<int>& neighborChunk = it->second;
                            int neighborBlockIndex = neighborLocalX + (ny * CHUNK_X_DIM) + (neighborLocalZ * CHUNK_X_DIM * CHUNK_Y_DIM);
                            shouldRenderFace = (neighborChunk[neighborBlockIndex] < 0);
                        } else {
                            shouldRenderFace = true;
                        }
                    } else {
                        shouldRenderFace = (blocksByPosition[nx + (ny * CHUNK_X_DIM) + (nz * CHUNK_X_DIM * CHUNK_Y_DIM)] < 0);
                    }

                    if (shouldRenderFace) {
                        int vertexOffset = vertices.size();
                        for (int j = 0; j < 4; ++j) {
                            Vertex vertex;
                            vertex.position = glm::vec3(
                                cubeVertices[face * 12 + j * 3] + x + (CHUNK_X_DIM * xPos) - ((float)CHUNK_X_DIM * WORLD_X_DIM / 2),
                                cubeVertices[face * 12 + j * 3 + 1] + y,
                                cubeVertices[face * 12 + j * 3 + 2] + z + (CHUNK_Z_DIM * zPos) - ((float)CHUNK_Z_DIM * WORLD_X_DIM / 2));
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

