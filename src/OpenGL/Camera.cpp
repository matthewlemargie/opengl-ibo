#include"Camera.h"

Camera::Camera(GLContext* glContext, glm::vec3 position, float FOVdeg, float nearPlane, float farPlane)
: glContext(glContext), width(glContext->mode->width), height(glContext->mode->height),Position(position), initialPosition(position),
FOVdeg(FOVdeg), nearPlane(nearPlane), farPlane(farPlane), mouseX(0.0f), mouseY(0.0f), frustumShader("shaders/ray_vert.glsl", "shaders/ray_frag.glsl")
{
    glfwSetInputMode(glContext->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
    normalSpeed = 0.5f * (float)(60.0 * timeDiff);
	fastSpeed = 2.0f * normalSpeed;
    startTime = glfwGetTime();

    if (glfwGetKey(glContext->window, GLFW_KEY_ESCAPE) == GLFW_PRESS && !firstEscapeClick) {
        escapeMode = !escapeMode;
        if (escapeMode)
            glfwSetInputMode(glContext->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        else
            glfwSetInputMode(glContext->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        firstEscapeClick = true;
    }
    if (glfwGetKey(glContext->window, GLFW_KEY_ESCAPE) == GLFW_RELEASE) {
        firstEscapeClick = false;
    }

    if (!escapeMode) {
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

        // Get the current mouse position
        glfwGetCursorPos(glContext->window, &mouseX, &mouseY);

        // Calculate the difference in mouse movement since the last frame
        float rotX = sensitivity * (float)(mouseY - height / 2.0) / height;
        float rotY = sensitivity * (float)(mouseX - width / 2.0) / width;

        // Update the rotation based on the movement
        glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));
        if (!(glm::angle(newOrientation, Up) <= glm::radians(5.0f) || glm::angle(newOrientation, -Up) <= glm::radians(5.0f)))
        {
            Orientation = newOrientation;
        }

        // Apply Y-axis rotation
        Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

        // Reset cursor position to center for the next frame
        glfwSetCursorPos(glContext->window, (float)width / 2.0f, (float)height / 2.0f);

    }
}

void Frustum::update(const Camera& camera) {
    // Calculate projection and view matrices

    // Combine the view and projection matrices
    glm::mat4 vpMatrix = camera.projection * camera.view;

    // Extract the frustum planes from the view-projection matrix
    extractFrustumPlanes(vpMatrix);
}

void Frustum::extractFrustumPlanes(const glm::mat4& vpMatrix) {
    // Left plane:  = vpMatrix[0][3] + vpMatrix[0][0], vpMatrix[1][3] + vpMatrix[1][0], vpMatrix[2][3] + vpMatrix[2][0], vpMatrix[3][3] + vpMatrix[3][0]
    planes[0].normal.x = vpMatrix[0][3] + vpMatrix[0][0];
    planes[0].normal.y = vpMatrix[1][3] + vpMatrix[1][0];
    planes[0].normal.z = vpMatrix[2][3] + vpMatrix[2][0];
    planes[0].distance = vpMatrix[3][3] + vpMatrix[3][0];

    // Right plane: = vpMatrix[0][3] - vpMatrix[0][0], etc.
    planes[1].normal.x = vpMatrix[0][3] - vpMatrix[0][0];
    planes[1].normal.y = vpMatrix[1][3] - vpMatrix[1][0];
    planes[1].normal.z = vpMatrix[2][3] - vpMatrix[2][0];
    planes[1].distance = vpMatrix[3][3] - vpMatrix[3][0];

    // Top plane: = vpMatrix[0][3] - vpMatrix[0][1], etc.
    planes[2].normal.x = vpMatrix[0][3] - vpMatrix[0][1];
    planes[2].normal.y = vpMatrix[1][3] - vpMatrix[1][1];
    planes[2].normal.z = vpMatrix[2][3] - vpMatrix[2][1];
    planes[2].distance = vpMatrix[3][3] - vpMatrix[3][1];

    // Bottom plane: = vpMatrix[0][3] + vpMatrix[0][1], etc.
    planes[3].normal.x = vpMatrix[0][3] + vpMatrix[0][1];
    planes[3].normal.y = vpMatrix[1][3] + vpMatrix[1][1];
    planes[3].normal.z = vpMatrix[2][3] + vpMatrix[2][1];
    planes[3].distance = vpMatrix[3][3] + vpMatrix[3][1];

    // Near plane: = vpMatrix[0][3] + vpMatrix[0][2], etc.
    planes[4].normal.x = vpMatrix[0][3] + vpMatrix[0][2];
    planes[4].normal.y = vpMatrix[1][3] + vpMatrix[1][2];
    planes[4].normal.z = vpMatrix[2][3] + vpMatrix[2][2];
    planes[4].distance = vpMatrix[3][3] + vpMatrix[3][2];

    // Far plane: = vpMatrix[0][3] - vpMatrix[0][2], etc.
    planes[5].normal.x = vpMatrix[0][3] - vpMatrix[0][2];
    planes[5].normal.y = vpMatrix[1][3] - vpMatrix[1][2];
    planes[5].normal.z = vpMatrix[2][3] - vpMatrix[2][2];
    planes[5].distance = vpMatrix[3][3] - vpMatrix[3][2];

    // Normalize each plane
    for (int i = 0; i < 6; i++) {
        float len = glm::length(planes[i].normal);
        planes[i].normal /= len;
        planes[i].distance /= len;
    }
}

bool Frustum::pointInFrustum(const glm::vec3& point) const {
    for (const auto& plane : planes) {
        if (glm::dot(plane.normal, point) + plane.distance < 0)
            return false;
    }
    return true;
}

// Function to check if an AABB intersects the frustum
bool Frustum::intersectsAABB(const glm::vec3& min, const glm::vec3& max) const {
    for (const auto& plane : planes) {
        // Select the positive and negative vertex for the plane
        glm::vec3 positive(
            plane.normal.x >= 0 ? max.x : min.x,
            plane.normal.y >= 0 ? max.y : min.y,
            plane.normal.z >= 0 ? max.z : min.z
        );

        glm::vec3 negative(
            plane.normal.x >= 0 ? min.x : max.x,
            plane.normal.y >= 0 ? min.y : max.y,
            plane.normal.z >= 0 ? min.z : max.z
        );

        // If the positive vertex is outside, the entire AABB is outside
        if (glm::dot(plane.normal, positive) + plane.distance < 0)
            return false; // Fully outside, cull this chunk
    }
    return true; // At least part of the AABB is inside the frustum
}
