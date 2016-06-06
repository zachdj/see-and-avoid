#pragma once

#include "Aircraft.h"
#include "Camera.h"
#include "Shader.h"

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class AircraftDrawer
{
public:
	AircraftDrawer(Texture & tex, Shader & aircraftShader);
	void Draw(glm::mat4 view, glm::mat4 projection, GLfloat timeValue, std::vector<Aircraft> & toDraw);

	Texture tex; // this texture will be used as a fallback when the model has no associated texture data
	Shader shader;
	~AircraftDrawer();
};

