#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "ModelLoader.h"
#include "VAO.h"
#include "VBO.h"
#include "IBO.h"
#include "EBO.h"
#include "Texture.h"
#include "Camera.h"
#include "AABB.h"


struct Mesh
{
    glm::mat4 model = glm::mat4(1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, 1.0f);

    float speed = 0.025f;
    float sensitivity = 100.0f;

    bool firstClick = true;

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    GLfloat* faceVertices;
    GLuint* faceIndices;
    size_t faceVertexSize;
    size_t faceIndexSize;

    std::vector<glm::mat4> instanceMats;
    AABB modelAABB;

    VAO vao;
    VBO vbo;
    IBO ibo;

    float* scale;
    glm::vec4* lightColor;
    glm::vec3* lightPos;
    
    Shader* shader;

    int numInstances = 0;

    Mesh(const std::string& model, Shader& shader, float& scale);
    Mesh(GLfloat* vertices, size_t vertexSize, GLuint* indices, size_t indexSize, Shader& shader, float& scale);

    void addInstance(std::vector<glm::mat4> instanceMats);
    void addInstance(glm::mat4 instanceMat);
    void Draw(Camera& camera);

    void Inputs(GLFWwindow* window);
};

#endif

