#include "block.h"

Block::Block(float& scale) 
: scale(&scale) 
{
    vao.Bind();
    vbo.addVertices(cubeVertices, sizeof(cubeVertices));
    EBO ebo(cubeIndices, sizeof(cubeIndices));

    vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    vao.LinkAttrib(vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	vao.LinkInstance(ibo);

    vao.Unbind();
    vbo.Unbind();
    ebo.Unbind();

    modelAABB = AABB(scale * glm::vec3(-0.5f, -0.5f, -0.5f), scale * glm::vec3(0.5f, 0.5f, 0.5f));

    grass = new Texture("assets/textures/grass.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    shader = new Shader("shaders/face_vert.glsl", "shaders/face_frag.glsl");
}

Block::~Block() {
    delete grass;
    delete shader;
}

void Block::addInstance(std::vector<glm::mat4> instanceMats) {
    vao.Bind();
    ibo.Bind();
    ibo.addInstance(instanceMats, modelAABB);
    ibo.Unbind();
    vao.Unbind();
}

void Block::textureActivate(Camera& camera) {
    grass->texUnit(*shader, "tex0", 0);
    grass->Bind();
}

void Block::drawCube() {
	vao.Bind();
	glDrawElementsInstanced(GL_TRIANGLES, sizeof(cubeIndices) / sizeof(GLuint), GL_UNSIGNED_INT, 0, ibo.instances.size());
    vao.Unbind();
}

void Block::drawTop() {
	vao.Bind();
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(24 * sizeof(GLuint)), ibo.instances.size());
    vao.Unbind();
}
void Block::drawBottom() {
	vao.Bind();
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(30 * sizeof(GLuint)), ibo.instances.size());
    vao.Unbind();
}
void Block::drawNorth() {
	vao.Bind();
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(0 * sizeof(GLuint)), ibo.instances.size());
    vao.Unbind();
}
void Block::drawSouth() {
	vao.Bind();
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(6 * sizeof(GLuint)), ibo.instances.size());
    vao.Unbind();
}
void Block::drawEast() {
	vao.Bind();
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(18 * sizeof(GLuint)), ibo.instances.size());
    vao.Unbind();
}
void Block::drawWest() {
	vao.Bind();
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(12 * sizeof(GLuint)), ibo.instances.size());
    vao.Unbind();
}

