#include "Aircraft.h"

Aircraft::Aircraft(glm::vec3 position, GLchar* path) : model(path)
{
	this->position = position;
	this->velocityDirection = glm::vec3(1.0f, 0.0f, 0.0f);
	this->velocityMagnitude = 0;
	this->rotationAxis = glm::vec3(1.0f, 0.0f, 0.0f);
	this->rotationAngle = 0;
	this->angularVelocityAxis = glm::vec3(1.0f, 0.0f, 0.0f);
	this->angularVelocityMagnitude = 0;
	Model aircraftModel(path);
	this->model = aircraftModel;
}

void Aircraft::SetRotation(GLfloat angle, glm::vec3 axis)
{
	this->rotationAngle = angle;
	this->rotationAxis = axis;
}

void Aircraft::SetAngularVelocity(GLfloat deltaAngle, glm::vec3 axis)
{
	this->angularVelocityAxis = axis;
	this->angularVelocityMagnitude = deltaAngle;
}

void Aircraft::SetPosition(glm::vec3 position)
{
	this->position = position;
}

void Aircraft::SetVelocity(GLfloat newVelocity, glm::vec3 direction)
{
	this->velocityMagnitude = newVelocity;
	this->velocityDirection = glm::normalize(direction); // make direction vector unit vector
}

Aircraft::~Aircraft()
{
}
