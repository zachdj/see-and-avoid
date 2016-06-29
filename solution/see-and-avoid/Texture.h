
/*
Is used when building the model of an aircraft. We add a texture to it to help give us geometry. 
*/

#pragma once

#include <string>
#include <sstream>

#include <glew.h>; // Include glew to get all the required OpenGL headers

#include <SOIL.h>;

class Texture
{
public:
	// The texture ID
	GLuint texture;
	// Constructor reads and builds the texture from the given file
	Texture() {};
	Texture(const GLchar* texturePath);
	~Texture();
	void Use(GLuint textureUnit);
	static void Unbind();
};

