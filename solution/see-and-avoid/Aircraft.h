#pragma once

#include "Texture.h"
#include "Shader.h"

#include "Model.h"

class Aircraft
{
public:
	Aircraft(glm::vec3 position, GLfloat collisionRadius, GLchar* modelFilePath);
	void SetOrientation(GLfloat pitch, GLfloat yaw, GLfloat roll);
	void SetPosition(glm::vec3 position);
	void SetSpeed(GLfloat newSpeed);

	glm::vec3 position;
	GLfloat pitch, yaw, roll, speed;

	GLfloat collisionRadius;
	bool hasCollided; //keeps track of whether this aircraft has collided with the camera to prevent multiple reportings

	Model model;

	~Aircraft();

};

