#include"Camera.h"

Camera::Camera(GLContext* glContext, glm::vec3 position, float FOVdeg, float nearPlane, float farPlane)
: glContext(glContext), width(glContext->mode->width), height(glContext->mode->height),Position(position), initialPosition(position),
FOVdeg(FOVdeg), nearPlane(nearPlane), farPlane(farPlane), mouseX(0.0f), mouseY(0.0f), frustumShader("shaders/ray_vert.glsl", "shaders/ray_frag.glsl")
{

	hNear = this->nearPlane * tan(glm::radians(this->FOVdeg / 2.0f));
	wNear = hNear * ((float)width / height);
	hFar = this->farPlane * tan(glm::radians(this->FOVdeg / 2.0f));
	wFar = hFar * ((float)width / height);

    GLfloat vertices[] = {
        // Near Plane
        -wNear,  hNear, -this->nearPlane,  wNear,  hNear, -this->nearPlane,
         wNear,  hNear, -this->nearPlane,  wNear, -hNear, -this->nearPlane,
         wNear, -hNear, -this->nearPlane, -wNear, -hNear, -this->nearPlane,
        -wNear, -hNear, -this->nearPlane, -wNear,  hNear, -this->nearPlane,

        // Far Plane
        -wFar,  hFar, -this->farPlane,  wFar,  hFar, -this->farPlane,
         wFar,  hFar, -this->farPlane,  wFar, -hFar, -this->farPlane,
         wFar, -hFar, -this->farPlane, -wFar, -hFar, -this->farPlane,
        -wFar, -hFar, -this->farPlane, -wFar,  hFar, -this->farPlane,

        // Connect Near and Far Planes
        -wNear,  hNear, -this->nearPlane,  -wFar,   hFar,  -this->farPlane,
         wNear,  hNear, -this->nearPlane,   wFar,   hFar,  -this->farPlane,
         wNear, -hNear, -this->nearPlane,   wFar,  -hFar,  -this->farPlane,
        -wNear, -hNear, -this->nearPlane,  -wFar,  -hFar,  -this->farPlane
    };

    // glGenVertexArrays(1, &VAO);
    // glGenBuffers(1, &VBO);
    // glBindVertexArray(VAO);
    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    // glEnableVertexAttribArray(0);

    startTime = glfwGetTime();

}

void Camera::updateMatrix()
{
	view = glm::lookAt(Position, Position + Orientation, Up);
	float Width = width;
	float Height = height;
	float aspectRatio = Width / Height;
	projection = glm::perspective(glm::radians(FOVdeg), aspectRatio, nearPlane, farPlane);
	cameraMatrix = projection * view;
}

void Camera::Matrix(Shader& shader, const char* uniform)
{
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::drawFrustumFromCamera(Camera& camera) {
	frustumShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(frustumShader.ID, "camMatrix"), 1, GL_FALSE, glm::value_ptr(camera.cameraMatrix));
	glm::mat4 inverseView = glm::inverse(view);
	glUniformMatrix4fv(glGetUniformLocation(frustumShader.ID, "inverseView"), 1, GL_FALSE, glm::value_ptr(inverseView));

	// Draw the frustum
	glBindVertexArray(VAO);
	glLineWidth(1.0f);
	glDrawArrays(GL_LINES, 0, 24);
	glBindVertexArray(0);
}

void Camera::Inputs()
{
    endTime = glfwGetTime();
    timeDiff = endTime - startTime;
    normalSpeed = 0.25f * (float)(60.0 * timeDiff);
	fastSpeed = 2.0f * normalSpeed;
    startTime = glfwGetTime();

	if (glfwGetKey(glContext->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		speed = fastSpeed;
	}
	else if (glfwGetKey(glContext->window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		speed = normalSpeed;
	}

	if (glfwGetKey(glContext->window, GLFW_KEY_W) == GLFW_PRESS)
	{
		Position += speed * Orientation;
	}
	if (glfwGetKey(glContext->window, GLFW_KEY_A) == GLFW_PRESS)
	{
		Position += speed * -glm::normalize(glm::cross(Orientation, Up));
	}
	if (glfwGetKey(glContext->window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Position += speed * -Orientation;
	}
	if (glfwGetKey(glContext->window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Position += speed * glm::normalize(glm::cross(Orientation, Up));
	}
	if (glfwGetKey(glContext->window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		Position += speed * Up;
	}
	if (glfwGetKey(glContext->window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		Position += speed * -Up;
	}
	if (glfwGetKey(glContext->window, GLFW_KEY_O) == GLFW_PRESS)
	{
		Position = initialPosition;
		Orientation = initialOrientation;
	}

	double lastX = width / 2.0, lastY = height / 2.0;

	if (glfwGetMouseButton(glContext->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		glfwSetInputMode(glContext->window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		if (firstClick)
		{
			// Set the cursor position to the center of the screen only once
			glfwSetCursorPos(glContext->window, (float)width / 2.0f, (float)height / 2.0f);
			firstClick = false;
		}
		
		// Get the current mouse position
		glfwGetCursorPos(glContext->window, &mouseX, &mouseY);

		// Calculate the difference in mouse movement since the last frame
		float rotX = sensitivity * (float)(mouseY - lastY) / height;
		float rotY = sensitivity * (float)(mouseX - lastX) / width;  // Consider the width for X rotation

		// Update the rotation based on the movement
		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));
		if (!(glm::angle(newOrientation, Up) <= glm::radians(5.0f) || glm::angle(newOrientation, -Up) <= glm::radians(5.0f)))
		{
			Orientation = newOrientation;
		}

		// Apply Y-axis rotation
		Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

		// Save the current mouse position for the next frame
		lastX = mouseX;
		lastY = mouseY;

		// Reset cursor position to center for the next frame
		glfwSetCursorPos(glContext->window, (float)width / 2.0f, (float)height / 2.0f);
	}
	else if (glfwGetMouseButton(glContext->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		glfwSetInputMode(glContext->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstClick = true;
	}
}
