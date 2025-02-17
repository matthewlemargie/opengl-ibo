#define STB_IMAGE_IMPLEMENTATION
#include "Texture.h"

Texture::Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType)
{
    double startTime = glfwGetTime();
	type = texType;
	int widthImg, heightImg, numColCh;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

	glGenTextures(1, &ID);
	glActiveTexture(slot);
	glBindTexture(GL_TEXTURE_2D, ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.75f);

    // float maxAniso = 0.0f;
    // glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAniso); // Get max supported value
    // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, maxAniso);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(bytes);
	glBindTexture(GL_TEXTURE_2D, 0);
    double timeDiff = glfwGetTime() - startTime;
    cout << image << " loaded in " << timeDiff << "s" << endl;
}

void Texture::texUnit(Shader shader, const char* uniform, GLuint unit) {
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	shader.Activate();
	glUniform1i(texUni, unit);
}

void Texture::Bind()
{
    glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}
