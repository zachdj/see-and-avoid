#pragma once

/* 
This class creates an object of Aircraft. Each airplane we build in the sky will be an aircraft object.
*/

#include "Texture.h"
#include "Shader.h"

#include "Model.h"
#include "Path.h"

class Aircraft
{
public:
	Aircraft(glm::vec3 startingPos, Path & path, GLfloat collisionRadius, GLchar* modelFilePath, GLfloat scale);
	Aircraft(glm::vec3 position, GLfloat collisionRadius, GLchar* modelFilePath, GLfloat scale);
	glm::vec3 GetCurrentDirection();
	void SetOrientation(GLfloat pitch, GLfloat yaw, GLfloat roll);
	void SetPosition(glm::vec3 position);
	void SetSpeed(GLfloat newSpeed);
	bool IsAutonomous();
	Path * GetPath();

	glm::vec3 position;
	GLfloat pitch, yaw, roll, speed;

	GLfloat collisionRadius;
	bool hasCollided; //keeps track of whether this aircraft has collided with the camera to prevent multiple reportings

	Model model; // Model used for drawing this aircraft
	GLfloat scale;
	~Aircraft();

private:
	//if autonomous mode is truthy, then the UAV will navigate using its path.  Otherwise it will just follow its current orientation
	bool autonomousMode; // autonomous mode is set once in the constructor
	Path path;

};

