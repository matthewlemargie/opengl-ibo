#include "Mesh.h"

Mesh::Mesh(const std::string& model, Shader& shader, float& scale)
: shader(&shader), scale(&scale)
{
	Model modelData;
	modelData.load(model);
	vertices = modelData.getVertices();
	indices = modelData.getIndices();

    for (const auto& vertex : vertices) {
        modelAABB.expand(scale * vertex.position);
    }

	vao.Bind();

	vbo.addVertices(vertices);

	EBO ebo(indices);
	vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, position));
	vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	// vao.LinkAttrib(vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, texUV));
	vao.LinkInstance(ibo);

	vao.Unbind();
	vbo.Unbind();
	ebo.Unbind();
}

Mesh::Mesh(GLfloat* vertices, size_t vertexSize, GLuint* indices, size_t indexSize, Shader& shader, float& scale)
: shader(&shader), scale(&scale)
{
    this->faceVertices = vertices;
    this->faceIndices = indices;
    this->faceVertexSize = vertexSize;
    this->faceIndexSize = indexSize;

    vao.Bind();
    vbo.addVertices(vertices, vertexSize);
    EBO ebo(indices, indexSize);
    
    vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    vao.LinkAttrib(vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	vao.LinkInstance(ibo);

    vao.Unbind();
    vbo.Unbind();
    ebo.Unbind();
}

void Mesh::addInstance(std::vector<glm::mat4> instanceMats) {
    vao.Bind();
    ibo.Bind();
    ibo.addInstance(instanceMats, modelAABB);
    ibo.Unbind();
    vao.Unbind();
}

void Mesh::addInstance(glm::mat4 instanceMat) {
    vao.Bind();
    ibo.Bind();
    ibo.addInstance(instanceMat, modelAABB);
    ibo.Unbind();
    vao.Unbind();
}

void Mesh::Draw(Camera& camera)
{
	vao.Bind();
	glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, ibo.instances.size());
    vao.Unbind();
}

void Mesh::Inputs(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f) * speed);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f) * -speed);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f) * -speed);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f) * speed);
	}		
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		model = glm::translate(model, Up * speed);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		model = glm::translate(model, Up * -speed);
	}
// Declare lastX and lastY globally or at the beginning of your function

	int width, height;

	glfwGetWindowSize(window, &width, &height);
	
	double lastX = width / 2.0, lastY = height / 2.0;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		if (firstClick)
		{
			// Set the cursor position to the center of the screen only once
			glfwSetCursorPos(window, int(width / 2), int(height / 2));
			firstClick = false;
		}

		// Get the current mouse position
		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Calculate the difference in mouse movement since the last frame
		float rotX = sensitivity * (float)(mouseY - lastY) / height;
		float rotY = sensitivity * (float)(mouseX - lastX) / width;  // Consider the width for X rotation

		// Update the rotation based on the movement
		model = glm::rotate(model, glm::radians(-rotX), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::rotate(model, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));
		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));
		// Apply Y-axis rotation
		model = glm::rotate(model, glm::radians(rotY), Up);

		// Save the current mouse position for the next frame
		lastX = mouseX;
		lastY = mouseY;

		// Reset cursor position to center for the next frame
		glfwSetCursorPos(window, int(width / 2), int(height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstClick = true;
	}

}

//for (unsigned int i = 0; i < textures.size(); i++)
//{
//	std::string num;
//	std::string type = textures[i].type;
//	if (type == "diffuse")
//	{
//		num = std::to_string(numDiffuse++);
//	}
//	else if (type == "specular")
//	{
//		num = std::to_string(numSpecular++);
//	}
//	textures[i].texUnit(shader, (type + num).c_str(), i);
//	textures[i].Bind();
//}
