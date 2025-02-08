#include "skybox.h"


Skybox::Skybox(Camera& camera) 
: camera(&camera)
{
    skyboxShader = new Shader("skybox.vert", "skybox.frag");

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glGenBuffers(1, &skyboxEBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // std::string parentDir = (filesystem::current_path().filesystem::path::parent_path()).string();
    std::string parentDir = "/home/dmac/Documents/projects2025/opengl-ibo";
	// All the faces of the cubemap (make sure they are in this exact order)
	std::string facesCubemap[6] =
	{
        parentDir + "/assets/skyboxes/cloudy/blue/left.jpg",
        parentDir + "/assets/skyboxes/cloudy/blue/right.jpg",
        parentDir + "/assets/skyboxes/cloudy/blue/downflop.jpg",
        parentDir + "/assets/skyboxes/cloudy/blue/upflop.jpg",
        parentDir + "/assets/skyboxes/cloudy/blue/front.jpg",
        parentDir + "/assets/skyboxes/cloudy/blue/back.jpg",
	};

	// Creates the cubemap texture object
	glGenTextures(1, &cubemapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glActiveTexture(GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// These are very important to prevent seams
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    	// This might help with seams on some systems
	//glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	// Cycles through all the textures and attaches them to the cubemap object
	for (unsigned int i = 0; i < 6; i++)
	{
		int width, height, nrChannels;
		unsigned char* data = stbi_load(facesCubemap[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
            stbi_set_flip_vertically_on_load(true);
            // stbi_set_flip_vertically_on_load(false);
			glTexImage2D
			(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				GL_RGB,
				width,
				height,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Failed to load texture: " << facesCubemap[i] << std::endl;
			stbi_image_free(data);
		}
	}

	// glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Skybox::Draw() {
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDepthFunc(GL_LEQUAL);
    skyboxShader->Activate();
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    // We make the mat4 into a mat3 and then a mat4 again in order to get rid of the last row and column
    // The last row and column affect the translation of the skybox (which we don't want to affect)
    view = glm::mat4(glm::mat3(glm::lookAt(camera->Position, camera->Position + camera->Orientation, camera->Up)));
    projection = glm::perspective(glm::radians(60.0f), (float)1920 / 1080, 0.1f, 10000.0f);
    glUniformMatrix4fv(glGetUniformLocation(skyboxShader->ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(skyboxShader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // Draws the cubemap as the last object so we can save a bit of performance by discarding all fragments
    // where an object is present (a depth of 1.0f will always fail against any object's depth value)
    glDepthMask(GL_FALSE);
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glDepthMask(GL_TRUE);

    // Switch back to the normal depth function
    glDepthFunc(GL_LESS);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}
