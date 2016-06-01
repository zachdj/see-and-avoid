#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include <glew.h>; // Include glew to get all the required OpenGL headers
#include <glm/glm.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
	Camera();
	Camera(GLuint screenW, GLuint screenH);
	glm::mat4 GetCameraViewMatrix();
	glm::mat4 GetProjectionMatrix();
	~Camera();

	GLuint screenW;
	GLuint screenH;
};

#endif

