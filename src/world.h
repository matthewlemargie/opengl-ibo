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

#include <map>
#include <utility>  // For std::pair
#include <functional>  // For std::hash
#include <thread>
#include <mutex>

struct World {
    bool firstChunk = true;
    Frustum frustum;

    std::unordered_map<std::pair<int, int>, std::vector<int>, pair_hash> chunks;

    std::unordered_map<std::pair<int, int>, GLuint, pair_hash> chunkVAOs;
    std::unordered_map<std::pair<int, int>, GLuint, pair_hash> chunkVBOs;
    std::unordered_map<std::pair<int, int>, GLuint, pair_hash> chunkEBOs;
    std::unordered_map<std::pair<int, int>, size_t, pair_hash> chunkIndexCounts;

    chunkMesh chunkmaker;

    Texture* atlas;
    Shader* shader;
    World(GLContext* context);
    ~World();

    void textureActivate();
    void addChunkMeshToWorld(int xPos, int zPos, std::vector<Vertex> chunkVertices, std::vector<GLuint> chunkIndices);
    void addBlock(int blockID, int posInChunk, std::array<int,2> posInWorld);
    void deleteBlock(int posInChunk, std::array<int,2> posInWorld);
    void Render(Camera& camera);
};
#endif
