#include "Mesh.h"

Mesh::Mesh(const std::string& model)
{
	Model modelData;
	modelData.load(model);
	this->vertices = modelData.getVertices();
	this->indices = modelData.getIndices();


	vao.Bind();

	vbo.addVertices(vertices);

	EBO ebo(indices);
	vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, position));
	vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	vao.LinkInstance(ibo);

	vao.Unbind();
	vbo.Unbind();
	ebo.Unbind();
}

void Mesh::addInstance(std::vector<glm::mat4> instanceMats) {
    vao.Bind();
    ibo.Bind();
    ibo.addInstance(instanceMats);
    ibo.Unbind();
    vao.Unbind();
}

void Mesh::Draw(Shader& shader, Camera& camera, glm::vec3 lightPos, glm::vec4 lightColor)
{
	shader.Activate();
	//glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
	vao.Bind();
	glDrawElementsInstanced(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0, ibo.maxInstances);
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
			glfwSetCursorPos(window, width / 2, height / 2);
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
		glfwSetCursorPos(window, (width / 2), (height / 2));
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
