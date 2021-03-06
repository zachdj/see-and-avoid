
/*
Shader information that is used in OpenGL environment
*/

#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glew.h>; // Include glew to get all the required OpenGL headers

class Shader
{
public:
	// The program ID
	GLuint Program;
	// Constructor reads and builds the shaders from the given files
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	Shader() {}
	~Shader();
	// Use the program
	void Use();
};

