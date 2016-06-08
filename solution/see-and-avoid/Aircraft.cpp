#include "Aircraft.h"

Aircraft::Aircraft(glm::vec3 position, GLfloat collisionRadius, GLchar* filepath) : model(filepath)
{
	this->position = position;
	this->pitch = 0.0f;
	this->roll = 0.0f;
	this->yaw = 0.0f;
	Model aircraftModel(filepath);
	this->model = aircraftModel;

	this->hasCollided = false;
	this->collisionRadius = collisionRadius;
}

void Aircraft::SetOrientation(GLfloat pitch, GLfloat yaw, GLfloat roll)
{
	this->pitch = pitch;
	this->yaw = yaw;
	this->roll = roll;
}

void Aircraft::SetPosition(glm::vec3 position)
{
	this->position = position;
}

void Aircraft::SetSpeed(GLfloat newSpeed)
{
	this->speed = newSpeed;
}

Aircraft::~Aircraft()
{
}
