
/*
Draws the cube in space after it is created by the Cube object.
*/

#pragma once

#include "Cube.h"
#include "Camera.h"
#include "Shader.h"

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class CubeDrawer 
{
public:
	CubeDrawer(Texture & tex1, Texture & tex2, Shader & defaultShader);
	void Draw(glm::mat4 view, glm::mat4 projection, GLfloat timeValue, std::vector<Cube> & toDraw);
	~CubeDrawer();

	Texture tex1;
	Texture tex2;
	GLuint VAO;
	Shader defaultShader;
};

