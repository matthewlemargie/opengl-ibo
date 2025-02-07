#include "block.h"

Block::Block(float& scale, glm::vec4& lightColor, glm::vec3& lightPos) 
    : scale(&scale), lightColor(&lightColor), lightPos(&lightPos) {
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

    grass = new Texture("grass.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    shader = new Shader("face.vert", "face.frag");
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

void Block::shaderCameraTextureActivate(Camera& camera) {
    grass->texUnit(*shader, "tex0", 0);
    grass->Bind();
    shader->Activate();
    glUniform1f(glGetUniformLocation(shader->ID, "scale"), *scale);
    glUniform4f(glGetUniformLocation(shader->ID, "lightColor"), lightColor->x, lightColor->y, lightColor->z, lightColor->w);
    glUniform3f(glGetUniformLocation(shader->ID, "lightPos"), lightPos->x, lightPos->y, lightPos->z);
    camera.Matrix(*shader, "camMatrix");
}

void Block::drawCube(Camera& camera) {
    shaderCameraTextureActivate(camera);
	vao.Bind();
	glDrawElementsInstanced(GL_TRIANGLES, sizeof(cubeIndices) / sizeof(GLuint), GL_UNSIGNED_INT, 0, ibo.instances.size());
    vao.Unbind();
}

void Block::drawTop(Camera& camera) {
    shaderCameraTextureActivate(camera);
	vao.Bind();
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(24 * sizeof(GLuint)), ibo.instances.size());
    vao.Unbind();
}
void Block::drawBottom(Camera& camera) {
    shaderCameraTextureActivate(camera);
	vao.Bind();
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(30 * sizeof(GLuint)), ibo.instances.size());
    vao.Unbind();
}
void Block::drawNorth(Camera& camera) {
    shaderCameraTextureActivate(camera);
	vao.Bind();
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(0 * sizeof(GLuint)), ibo.instances.size());
    vao.Unbind();
}
void Block::drawSouth(Camera& camera) {
    shaderCameraTextureActivate(camera);
	vao.Bind();
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(6 * sizeof(GLuint)), ibo.instances.size());
    vao.Unbind();
}
void Block::drawEast(Camera& camera) {
    shaderCameraTextureActivate(camera);
	vao.Bind();
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(18 * sizeof(GLuint)), ibo.instances.size());
    vao.Unbind();
}
void Block::drawWest(Camera& camera) {
    shaderCameraTextureActivate(camera);
	vao.Bind();
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(12 * sizeof(GLuint)), ibo.instances.size());
    vao.Unbind();
}

