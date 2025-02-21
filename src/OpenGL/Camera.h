#define GLM_ENABLE_EXPERIMENTAL
#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include <array>

#include "shaderClass.h"
#include "GLContext.h"

struct Plane {
    glm::vec3 normal;
    float distance;
};

struct Camera
{
    GLContext* glContext;

	glm::vec3 Position;
	glm::vec3 initialPosition;
	glm::vec3 Orientation = glm::vec3(-0.5f, 0.0f, -0.5f);
	glm::vec3 initialOrientation = glm::vec3(-0.5f, 0.0f, -0.5f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::mat4 cameraMatrix = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	float FOVdeg, nearPlane, farPlane;
	float hNear = 1.0f;
	float hFar = 1.0f;
	float wNear= 1.0f;
	float wFar = 1.0f;
	// float normalSpeed = 3.0f * (float)(60.0 / glContext->mode->refreshRate);
	// float fastSpeed = 2.0f * normalSpeed;
	float normalSpeed;
	float fastSpeed;
    float speed;
	float sensitivity = 100.0f;

    bool escapeMode = false;
    bool firstEscapeClick = false;

    int width;
    int height;

    double mouseX = 1.0f;
    double mouseY = 1.0f;

	bool firstClick = true;

	GLfloat vertices[72];

    GLuint VAO;

    Shader frustumShader;

    double startTime = 0.0;
    double endTime = 0.0;
    double timeDiff = 0.0;

	Camera(GLContext* glContext, glm::vec3 position, float FOVdeg, float nearPlane, float farPlane);

	void updateMatrix();
	void Matrix(Shader& shader, const char* uniform);
	void Inputs();
    void drawFrustumFromCamera(Camera& camera);
};

struct Frustum {
    std::array<Plane, 6> planes;

    void update(const Camera& camera);

    void extractFrustumPlanes(const glm::mat4& vpMatrix);

    bool pointInFrustum(const glm::vec3& point) const;

    // Function to check if an AABB intersects the frustum
    bool intersectsAABB(const glm::vec3& min, const glm::vec3& max) const;
};

#endif // !CAMERA_CLASS_H
