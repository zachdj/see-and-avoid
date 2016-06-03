#pragma once

#include <glew.h>; // Include glew to get all the required OpenGL headers
#include <glm\glm.hpp>

#include "Shader.h"

class IObstacle
{
public:
	virtual ~IObstacle() {}
	virtual void SetRotation(GLfloat angle, glm::vec3 axis) = 0;
	virtual void SetAngularVelocity(GLfloat angle, glm::vec3 axis) = 0;

	virtual void SetPosition(glm::vec3 position) = 0;
	virtual void SetVelocity(GLfloat newVelocity, glm::vec3 direction) = 0;

	glm::vec3 position;
	float velocityMagnitude; //how fast to go
	glm::vec3 velocityDirection; //what direction to go

	glm::vec3 rotationAxis; // axis to rotate around; e.g. (1.0f, 0.0f, 0.0f) will rotate around the x axis
	float rotationAngle; // constant angle of rotation
	glm::vec3 angularVelocityAxis;
	float angularVelocityMagnitude; // change in angle per timestep

};