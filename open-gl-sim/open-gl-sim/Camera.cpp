#include "Camera.h"

Camera::Camera()
{
	this->screenW = 800;
	this->screenH = 800;
}

Camera::Camera(GLuint screenW, GLuint screenH)
{
	this->screenW = screenW;
	this->screenH = screenH;
}

glm::mat4 Camera::GetCameraViewMatrix()
{
	//TODO
	glm::mat4 view;
	return glm::translate(view, glm::vec3(0.0f, 0.0f, -25.0f)); // move the camera (base position is -25.0f backwards on Z-axis
}

glm::mat4 Camera::GetProjectionMatrix()
{
	//TODO	
	return glm::perspective(glm::radians(45.0f), (GLfloat)this->screenW / (GLfloat)this->screenH, 0.1f, 100.0f); // GLM handles creation of perspective transformation matrix
}


Camera::~Camera()
{
}
