#include "blocks.h"

Blocks::Blocks(GLContext* context) 
{
    double startTime = glfwGetTime();
    shader = new Shader("shaders/block_vert.glsl", "shaders/block_frag.glsl");
    grass = new Texture("assets/textures/grass.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    modelAABB = AABB(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f));

    for (GLuint i = 0; i < BLOCKID_COUNT; ++i){
        GLuint vao, vbo, ebo, ibo;
        // generate vao
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        // generate vbo for positions/normals/blockTexCoords
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        // bind cubeVertices to buffer
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
        // positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribDivisor(0, 0);
        // normals
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        glVertexAttribDivisor(1, 0);
        // blockTexCoords
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
        glVertexAttribDivisor(2, 0);
        // generate ebo for indices
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), &cubeIndices, GL_STATIC_DRAW);
        // generate ibo for instances
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ARRAY_BUFFER, ibo);
        glBufferData(GL_ARRAY_BUFFER, (sizeof(float) * 3), nullptr, GL_STATIC_DRAW);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (void*)0);
        glEnableVertexAttribArray(3);
        glVertexAttribDivisor(3, 1);

        blocktypeToBuffersIDsMap.insert({i, {vao, ebo, ibo}});
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    double timeDiff = glfwGetTime() - startTime;
}

Blocks::~Blocks() {
    delete grass;
    delete shader;
}

void Blocks::addInstance(int blockID, std::array<int,3> posInWorld) {
    // vao.Bind();
    // GLuint iboID = blocktypeToBufferIDsMap[blockID][1];
    // glBindBuffer(GL_ARRAY_BUFFER, iboID);
    // vao.Unbind();
}

void Blocks::addChunkToWorld(std::vector<std::vector<GLfloat>> newBlockInstances) {
    // vao.Bind();
    GLuint vao, iboID;
    GLint bufferSize;

    std::vector<GLfloat> data;
    for (int i = 0; i < BLOCKID_COUNT; ++i) {
        std::vector<GLfloat> blockInstances = newBlockInstances[i];
        data = {};
        vao = blocktypeToBuffersIDsMap[i][0];
        iboID = blocktypeToBuffersIDsMap[i][2];
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, iboID);
        if (!firstChunk) {
            glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);
            GLfloat* mappedBuffer = (GLfloat*)glMapBufferRange(GL_ARRAY_BUFFER, 0, bufferSize, 
                    GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);
            std::cout << 3 << std::endl;
            if (mappedBuffer) {
                data.assign(mappedBuffer, mappedBuffer + (bufferSize / (sizeof(GLfloat))));
                glUnmapBuffer(GL_ARRAY_BUFFER);
            }
            data.insert(data.end(), blockInstances.begin(), blockInstances.end());

            // Debugging output
            std::cout << "Block ID " << i << " - Total instances: " << data.size() / 3 << std::endl;

            glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), data.data(), GL_STATIC_DRAW);
        }
        else {
            data = blockInstances;
            glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), data.data(), GL_STATIC_DRAW);
            firstChunk = false;
        }
    }
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Blocks::textureActivate() {
    grass->texUnit(*shader, "tex0", 0);
    grass->Bind();
}

void Blocks::Render(Camera& camera) {
    camera.Inputs();
    camera.updateMatrix();
    camera.Matrix(*shader, "camMatrix");
    skybox.Draw(&camera);

    textureActivate();
    GLuint vao, ebo, ibo;
    GLint bufferSize;
    for (int i = 0; i < BLOCKID_COUNT; ++i) {
        vao = blocktypeToBuffersIDsMap[i][0];
        ebo = blocktypeToBuffersIDsMap[i][1];
        ibo = blocktypeToBuffersIDsMap[i][2];

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, ibo);
        glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

        if (bufferSize > 0) {
            GLint instanceCount = bufferSize / (3 * sizeof(GLfloat));
            glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, 4096);
        }
    }
    glBindVertexArray(0);
}


std::vector<std::vector<GLfloat>> Blocks::populateChunk() {
    std::vector<std::vector<GLfloat>> newBlockInstances(BLOCKID_COUNT);  // Direct initialization
    
    auto start = std::chrono::high_resolution_clock::now();

    // Ensure you only iterate once for the chunk (16x16x16 blocks)
    for (int i = 0; i < CHUNK_X_DIM; ++i) {
        for (int j = 0; j < CHUNK_Y_DIM; ++j) {
            for (int k = 0; k < CHUNK_Z_DIM; ++k) {
                
                // Choose blockID (if it remains constant, it's fine)
                int newBlockID = GRASS;

                // Add coordinates for the block at (i, j, k)
                newBlockInstances[newBlockID].push_back(GLfloat(i));
                newBlockInstances[newBlockID].push_back(GLfloat(j));
                newBlockInstances[newBlockID].push_back(GLfloat(k));
            }
        }
    }

    // Output size to debug
    for (int i = 0; i < BLOCKID_COUNT; ++i) {
        std::cout << "Block Type " << i << ": " << newBlockInstances[i].size() / 3 << " instances" << std::endl;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Chunk populated in " << duration.count() << " seconds" << std::endl;

    return newBlockInstances;
}
