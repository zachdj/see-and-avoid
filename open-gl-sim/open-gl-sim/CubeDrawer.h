#pragma once
#ifndef CUBEDRAWER_H
#define CUBEDRAWER_H

#include "Cube.h"
#include "Camera.h"
#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class CubeDrawer 
{
public:
	CubeDrawer(Camera & camera, Texture & tex1, Texture & tex2, Shader & defaultShader);
	void Draw(GLfloat timeValue, Cube cubes[], GLuint count);
	~CubeDrawer();

	Texture tex1;
	Texture tex2;
	GLuint VAO;
	Camera camera;
	Shader defaultShader;
};

#endif

