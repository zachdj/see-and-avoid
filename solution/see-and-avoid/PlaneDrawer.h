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

private:
	static const GLfloat YAW_PER_ROLL; // how much does the yaw change for every degree of roll?
	static const GLfloat MAX_NG_ROLL_VELOCITY;
	
};

