#include "Texture.h"

Texture::Texture(const GLchar* texturePath) {
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// Set the texture wrapping/filtering options (on the currently bound texture object)
	// TODO
	// Load and generate the texture
	int width, height;
	unsigned char* image = SOIL_load_image(texturePath, &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	this->texture = texture;
}

// textureUnit must be one of the openGL texture units: GL_TEXTURE0, GL_TEXTURE1, ... GL_TEXTURE15
void Texture::Use(GLuint textureUnit) {
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, this->texture);
}

void Texture::Unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
	//glDeleteProgram(this->Program);
}