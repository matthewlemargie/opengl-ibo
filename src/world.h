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

struct World {
    bool firstChunk = true;

    GLuint vao;
    GLuint vbo;
    GLuint ebo;

    Texture* atlas;
    Shader* shader;

    World(GLContext* context);
    ~World();

    void textureActivate();
    void addChunkMeshToWorld(std::vector<GLfloat> chunkVertices, std::vector<GLuint> chunkIndices);
    void addBlock(int blockID, int posInChunk, std::array<int,2> posInWorld);
    void deleteBlock(int posInChunk, std::array<int,2> posInWorld);
    void Render(Camera& camera);
};
#endif
