#include "Aircraft.h"

Aircraft::Aircraft(glm::vec3 startingPos, Path & path, GLchar * modelFilePath, AircraftScale acScale) : model(modelFilePath)
{
	this->position = startingPos;
	this->pitch = 0.0f;
	this->roll = 0.0f;
	this->yaw = 0.0f;
	Model aircraftModel(modelFilePath);
	this->model = aircraftModel;

	double scale;
	if (acScale == AircraftScale::big) {
		scale = AircraftTable::getBestCase().wingspan / AircraftTable::GetPlaneModelWingspan();
		this->collisionRadius = AircraftTable::GetPlaneModelWingspan();
	}
	else if (acScale == AircraftScale::med) {
		scale = AircraftTable::getTypicalCase().wingspan / AircraftTable::GetFighterModelWingspan();
		this->collisionRadius = AircraftTable::GetFighterModelWingspan();
	}
	else {
		scale = AircraftTable::getWorstCase().wingspan / AircraftTable::GetCessnaModelWingspan();
		this->collisionRadius = AircraftTable::GetCessnaModelWingspan();
	}
	this->scale = scale;

	this->hasCollided = false;

	this->path = path;

	this->autonomousMode = true;

}

Aircraft::Aircraft(glm::vec3 position, GLchar* filepath, AircraftScale acScale) : model(filepath)
{
	this->position = position;
	this->pitch = 0.0f;
	this->roll = 0.0f;
	this->yaw = 0.0f;
	Model aircraftModel(filepath);
	this->model = aircraftModel;

	double scale;
	if (acScale == AircraftScale::big) {
		scale = AircraftTable::getBestCase().wingspan / AircraftTable::GetPlaneModelWingspan();
		this->collisionRadius = AircraftTable::getBestCase().wingspan;
	} else if (acScale == AircraftScale::med) {
		scale = AircraftTable::getTypicalCase().wingspan / AircraftTable::GetFighterModelWingspan();
		this->collisionRadius = AircraftTable::getTypicalCase().wingspan;
	} else {
		scale = AircraftTable::getWorstCase().wingspan / AircraftTable::GetCessnaModelWingspan();
		this->collisionRadius = AircraftTable::getWorstCase().wingspan;
	}
	this->scale = scale;

	this->hasCollided = false;

	this->autonomousMode = false;
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

glm::vec3 Aircraft::GetCurrentDirection() {
	// determine aircraft direction from pitch, roll, and velocity and update position
	glm::vec3 direction;
	direction.x = cos(glm::radians(this->pitch)) * cos(glm::radians(this->yaw + 90));
	direction.y = sin(glm::radians(this->pitch));
	direction.z = cos(glm::radians(this->pitch)) * sin(glm::radians(this->yaw + 90));
	return glm::normalize(direction);
}

bool Aircraft::IsAutonomous()
{
	return this->autonomousMode;
}

Path * Aircraft::GetPath()
{
	return &this->path;
}

Aircraft::~Aircraft()
{
}
