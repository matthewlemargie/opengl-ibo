#include "blocks.h"

Blocks::Blocks(int xDim, int yDim, int zDim, int totalBlocks, int totalBlockIDs) 
: blocktypeToBufferIDsMap(), xDim(xDim), yDim(yDim), zDim(zDim), totalBlocks(totalBlocks), totalBlockIDs(totalBlockIDs)
{
    double startTime = glfwGetTime();
    vao.Bind();
    vbo.addVertices(cubeVertices, sizeof(cubeVertices));
    EBO ebo(cubeIndices, sizeof(cubeIndices));

    vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void*)0);
    vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    
    for (unsigned int i = 0; i < totalBlockIDs; ++i){
        GLuint texvbo, ibo;
        glGenBuffers(1, &texvbo);
        glBindBuffer(GL_ARRAY_BUFFER, texvbo);
        glBufferData(GL_ARRAY_BUFFER, (sizeof(int) * 3) * totalBlocks, &blockTexCoords[48 * totalBlockIDs], GL_STATIC_DRAW);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, (void*)0);
        glVertexAttribDivisor(2, 1);

        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ARRAY_BUFFER, ibo);
        glBufferData(GL_ARRAY_BUFFER, (sizeof(int) * 3) * totalBlocks, nullptr, GL_STATIC_DRAW);
        glVertexAttribPointer(3, 3, GL_INT, GL_FALSE, sizeof(int) * 3, (void*)0);
        glVertexAttribDivisor(3, 1);

        blocktypeToBufferIDsMap.insert({i, {texvbo, ibo}});
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    vao.Unbind();
    vbo.Unbind();
    ebo.Unbind();

    modelAABB = AABB(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f));

    shader = new Shader("shaders/face_vert.glsl", "shaders/face_frag.glsl");
    grass = new Texture("assets/textures/grass.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    double timeDiff = glfwGetTime() - startTime;
    cout << "Blocks " << vao.ID << " initialized in " << timeDiff << "s" << endl;
}

Blocks::~Blocks() {
    delete grass;
    delete shader;
}

void Blocks::addInstance(int blockID, std::array<int,3> posInWorld) {
    vao.Bind();
    GLuint iboID = blocktypeToBufferIDsMap[blockID][1];
    glBindBuffer(GL_ARRAY_BUFFER, iboID);
    // glBufferSubData(GL_ARRAY_BUFFER, (sizeof(int) * 3) * totalBlocks, &blockTexCoords[48 * totalBlockIDs], GL_STATIC_DRAW);
    vao.Unbind();
}

void Blocks::textureActivate(Camera& camera) {
    grass->texUnit(*shader, "tex0", 0);
    grass->Bind();
}

void Blocks::Draw() {
	vao.Bind();
    vbo.Bind();
	glDrawElementsInstanced(GL_TRIANGLES, sizeof(cubeIndices) / sizeof(GLuint), GL_UNSIGNED_INT, 0, ibo.numInstances);
    vbo.Unbind();
    vao.Unbind();
}
