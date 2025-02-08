#ifndef SKYBOX_CLASS_H
#define SKYBOX_CLASS_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <stb/stb_image.h>
#include <string>
#include <iostream>

#include "Camera.h"
#include "shaderClass.h"

#include<filesystem>

struct Skybox {
    GLfloat skyboxVertices[24] =
    {
        //   Coordinates
        -1.0f, -1.0f,  1.0f,//        7--------6
         1.0f, -1.0f,  1.0f,//       /|       /|
         1.0f, -1.0f, -1.0f,//      4--------5 |
        -1.0f, -1.0f, -1.0f,//      | |      | |
        -1.0f,  1.0f,  1.0f,//      | 3------|-2
         1.0f,  1.0f,  1.0f,//      |/       |/
         1.0f,  1.0f, -1.0f,//      0--------1
        -1.0f,  1.0f, -1.0f
    };

    GLuint skyboxIndices[36] =
    {
        // Right
        1, 2, 6,
        6, 5, 1,
        // Left
        0, 4, 7,
        7, 3, 0,
        // Top
        4, 5, 6,
        6, 7, 4,
        // Bottom
        0, 3, 2,
        2, 1, 0,
        // Back
        0, 1, 5,
        5, 4, 0,
        // Front
        3, 7, 6,
        6, 2, 3
    };

	GLuint skyboxVAO, skyboxVBO, skyboxEBO;
	GLuint cubemapTexture;

    Camera* camera;
    Shader* skyboxShader;

    Skybox();
    void Draw(Camera* camera);

};

#endif
