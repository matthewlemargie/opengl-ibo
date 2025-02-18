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

#include "OpenGL/VAO.h"
#include "OpenGL/VBO.h"
#include "OpenGL/EBO.h"
#include "OpenGL/IBO.h"
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

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& pair) const {
        return std::hash<T1>()(pair.first) ^ (std::hash<T2>()(pair.second) << 1);
    }
};
struct World {
    bool firstChunk = true;

    GLuint vao;
    GLuint vbo;
    GLuint ebo;

    chunkMesh ChunkMesh;
    Chunk chunkmaker;

    Texture* atlas;
    Shader* shader;

    size_t currentVBOOffset = 0;
    size_t currentEBOOffset = 0;

    std::map<std::pair<int, int>, unsigned int> chunkIndexOffsets;

    World(GLContext* context);
    ~World();

    void textureActivate();
    void addChunkMeshToWorld(int xPos, int zPos, std::vector<GLfloat> chunkVertices, std::vector<GLuint> chunkIndices);
    void addBlock(int blockID, int posInChunk, std::array<int,2> posInWorld);
    void deleteBlock(int posInChunk, std::array<int,2> posInWorld);
    void Render(Camera& camera);
};
#endif
