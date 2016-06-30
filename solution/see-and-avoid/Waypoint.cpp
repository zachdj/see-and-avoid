#include "Waypoint.h"

Waypoint::Waypoint()
{
	this->position = glm::vec3(0.0f, 0.0f, 0.0f);
	this->active = true;
}

Waypoint::Waypoint(glm::vec3 position)
{
	this->position = position;
	this->active = true;
}

glm::vec3 Waypoint::GetPosition()
{
	return this->position;
}

void Waypoint::Activate() {
	this->active = true;
}

bool Waypoint::IsActive()
{
	return this->active;
}

void Waypoint::Complete() {
	this->active = false;
}


Waypoint::~Waypoint()
{
}
