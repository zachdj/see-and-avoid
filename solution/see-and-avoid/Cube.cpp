#include "Cube.h"

Cube::Cube(glm::vec3 position)
{
	this->position = position;
	this->velocityDirection = glm::vec3(1.0f, 0.0f, 0.0f);
	this->velocityMagnitude = 0;
	this->rotationAxis = glm::vec3(1.0f, 0.0f, 0.0f);
	this->rotationAngle = 0;
	this->angularVelocityAxis = glm::vec3(1.0f, 0.0f, 0.0f);
	this->angularVelocityMagnitude = 0;
}

void Cube::SetRotation(GLfloat angle, glm::vec3 axis)
{
	this->rotationAngle = angle;
	this->rotationAxis = axis;
}

void Cube::SetAngularVelocity(GLfloat deltaAngle, glm::vec3 axis)
{
	this->angularVelocityAxis = axis;
	this->angularVelocityMagnitude = deltaAngle;
}

void Cube::SetPosition(glm::vec3 position)
{
	this->position = position;
}

void Cube::SetVelocity(GLfloat newVelocity, glm::vec3 direction)
{
	this->velocityMagnitude = newVelocity;
	this->velocityDirection = glm::normalize(direction); // make direction vector unit vector
}

Cube::~Cube()
{
}
