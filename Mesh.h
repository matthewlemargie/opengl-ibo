#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "ModelLoader.h"
#include "VAO.h"
#include "IBO.h"
#include "EBO.h"
#include "Texture.h"
#include "Camera.h"


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
    std::vector<glm::mat4> instanceMats;

    VAO vao;
    VBO vbo;
    IBO ibo;

    int numInstances = 0;

    Mesh(const std::string& model);

    void addInstance(std::vector<glm::mat4> instanceMats);
    void Draw(Shader& shader, Camera& camera, glm::vec3 lightPos, glm::vec4 lightColor);

    void Inputs(GLFWwindow* window);
};

#endif

