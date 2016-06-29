
/*
Loads in the skybox (clouds) that are used in the OpenGl environment. It is easy to load in different skyboxes if
needed
*/

#pragma once

#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>

#include <iostream>
#include <cstdio>
#include <algorithm>
#include <vector>

#include <SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Cube.h"
#include "Camera.h"
#include "Shader.h"

using namespace std;

class Skybox
{
public:
	Skybox(Shader & shader);
	void Draw(glm::mat4 view, glm::mat4 projection);
	~Skybox();
	static GLuint loadCubemap(vector<const GLchar*> faces);
	Shader shader;
	GLuint VAO;
	GLuint textureID;

private:
};

