#pragma once
#ifndef CUBEDRAWER_H
#define CUBEDRAWER_H

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
	void Draw(glm::mat4 view, glm::mat4 projection, GLfloat timeValue, std::vector<Cube> & toDraw, GLuint count);
	~CubeDrawer();

	Texture tex1;
	Texture tex2;
	GLuint VAO;
	Shader defaultShader;
};

#endif

