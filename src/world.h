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

#include "OpenGL/Texture.h"
#include "OpenGL/shaderClass.h"
#include "OpenGL/Camera.h"
#include "OpenGL/skybox.h"
#include "AABB.h"
#include "chunk.h"
#include "constants.h"
#include "chunk.h"
#include "chunkMesh.h"
#include "OpenGL/Camera.h"

#include <map>
#include <utility>  // For std::pair
#include <functional>  // For std::hash
#include <thread>
#include <mutex>


struct World {
    World(GLContext* context);
    ~World();

    void updateChunks(Camera& camera);  // Handles dynamic chunk loading/unloading
    void loadChunksAround(int centerX, int centerZ);  // Loads chunks in a radius
    void removeChunkFromWorld(int xPos, int zPos);  // Removes old chunks
    void addChunkMeshToWorld(int xPos, int zPos, std::vector<Vertex> chunkVertices, std::vector<GLuint> chunkIndices);
    
    void textureActivate();
    void Render(Camera& camera);
    std::unordered_map<std::pair<int, int>, std::vector<int>, pair_hash> chunks;

    std::vector<std::thread> threads;
    std::mutex chunkMutex;

    Chunk chunkTemplate;
    Shader* shader;
    Texture* atlas;
    Frustum frustum;
    chunkMesh chunkmaker;

    std::unordered_map<std::pair<int, int>, GLuint, pair_hash> chunkVAOs;  // VAO storage
    std::unordered_map<std::pair<int, int>, GLuint, pair_hash> chunkVBOs;
    std::unordered_map<std::pair<int, int>, GLuint, pair_hash> chunkEBOs;
    std::unordered_map<std::pair<int, int>, int, pair_hash> chunkIndexCounts;  // Index count for each chunk

    std::pair<int, int> currentChunk = {0, 0};  // Player's current chunk
};

#endif // WORLD_H

