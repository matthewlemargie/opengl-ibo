#include "block.h"

Block::Block(Camera& camera) : camera(&camera) {
    vao.Bind();
    vbo.addVertices(cubeVertices, sizeof(cubeVertices));
    EBO ebo(cubeIndices, sizeof(cubeIndices));
    
    vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    vao.LinkAttrib(vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	vao.LinkFaceInstance(ibo);

    vao.Unbind();
    vbo.Unbind();
    ebo.Unbind();

    grass = new Texture("grass.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    shader = new Shader("face.vert", "face.frag");
}

Block::~Block() {
    delete grass;
    delete shader;
}

void Block::addFaceInstance(std::vector<glm::mat4> instanceMats) {
    vao.Bind();
    ibo.Bind();
    ibo.addInstance(instanceMats);
    ibo.Unbind();
    vao.Unbind();
}

void Block::shaderCameraTextureActivate() {
    shader->Activate();
    grass->texUnit(*shader, "tex0", 0);
    grass->Bind();
    camera->Matrix(*shader, "camMatrix");
}

void Block::drawCube() {
    shaderCameraTextureActivate();
	vao.Bind();
	glDrawElementsInstanced(GL_TRIANGLES, sizeof(cubeIndices) / sizeof(GLuint), GL_UNSIGNED_INT, 0, ibo.instances.size());
    vao.Unbind();
}

void Block::drawTop() {
    shaderCameraTextureActivate();
	vao.Bind();
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(24 * sizeof(GLuint)), ibo.instances.size());
    vao.Unbind();
}
void Block::drawBottom() {
    shaderCameraTextureActivate();
	vao.Bind();
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(30 * sizeof(GLuint)), ibo.instances.size());
    vao.Unbind();
}
void Block::drawNorth() {
    shaderCameraTextureActivate();
	vao.Bind();
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(0 * sizeof(GLuint)), ibo.instances.size());
    vao.Unbind();
}
void Block::drawSouth() {
    shaderCameraTextureActivate();
	vao.Bind();
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(6 * sizeof(GLuint)), ibo.instances.size());
    vao.Unbind();
}
void Block::drawEast() {
    shaderCameraTextureActivate();
	vao.Bind();
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(18 * sizeof(GLuint)), ibo.instances.size());
    vao.Unbind();
}
void Block::drawWest() {
    shaderCameraTextureActivate();
	vao.Bind();
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(12 * sizeof(GLuint)), ibo.instances.size());
    vao.Unbind();
}

