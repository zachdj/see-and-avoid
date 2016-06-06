#pragma once

#include "IObstacle.h"
#include "Texture.h"
#include "Shader.h"

class Cube :
	virtual public IObstacle
{
public:
	// tex1 and tex2 will be mixed to form the final box texture
	Cube(glm::vec3 position);
	void SetRotation(GLfloat angle, glm::vec3 axis);
	void SetAngularVelocity(GLfloat angle, glm::vec3 axis);

	void SetPosition(glm::vec3 position);
	void SetVelocity(GLfloat newVelocity, glm::vec3 direction);
	~Cube();

};

