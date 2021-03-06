
/*
Class draws the plane according to what the model tells us it looks like. We draw them in the OpenGL environment
*/


#pragma once

#include "Aircraft.h"
#include "Camera.h"
#include "Shader.h"
#include "PrintToFile.h"
#include "AircraftTable.h"

#include <vector>
#include <time.h>
#include <ctime>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class PlaneDrawer
{
public:
	PlaneDrawer(Texture & tex, Shader & planeShader);
	void Draw(Camera camera, glm::vec3 camPosition, GLfloat timeValue, std::vector<Aircraft*> & toDraw);

	GLfloat previousTimeStep;
	Texture tex; // this texture will be used as a fallback when the model has no associated texture data
	Shader shader;
	~PlaneDrawer();

	// this indicator will become true if we have recently collided with something head-on
	// used in main to check if we need to write out the current viewport
	tm * recentCollisionTime = nullptr;

private:
	static const GLfloat MAX_NG_ROLL_VELOCITY;
	GLfloat MAX_PITCH = 40.0f;
	GLfloat MAX_ROLL = 45.0f;
	int frontCollisionCount;
	int backCollisionCount;
	
};

