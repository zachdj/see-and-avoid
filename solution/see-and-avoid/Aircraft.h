#pragma once

#include "IObstacle.h"
#include "Texture.h"
#include "Shader.h"

#include "IObstacle.h"
#include "Model.h"

class Aircraft :
	public IObstacle
{
public:
	Aircraft(glm::vec3 position, GLchar* modelFilePath);
	void SetRotation(GLfloat angle, glm::vec3 axis);
	void SetAngularVelocity(GLfloat angle, glm::vec3 axis);

	void SetPosition(glm::vec3 position);
	void SetVelocity(GLfloat newVelocity, glm::vec3 direction);

	bool hasCollided; //keeps track of whether this aircraft has collided with the camera to prevent multiple reportings

	Model model;

	~Aircraft();
};

