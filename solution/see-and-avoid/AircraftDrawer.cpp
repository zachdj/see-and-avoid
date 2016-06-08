#include "AircraftDrawer.h"

AircraftDrawer::AircraftDrawer(Texture & tex, Shader & aircraftShader)
{
	this->tex = tex;
	this->shader = aircraftShader;
	this->previousTimeStep = 0;
}

void AircraftDrawer::Draw(glm::mat4 view, glm::mat4 projection, glm::vec3 camPosition, GLfloat timeValue, std::vector<Aircraft*> & toDraw)
{
	GLfloat timeDelta = timeValue - this->previousTimeStep;

	this->shader.Use();

	// load up textures
	this->tex.Use(GL_TEXTURE0);
	GLuint uniformLoc = glGetUniformLocation(this->shader.Program, "texture_diffuse1");
	glUniform1i(glGetUniformLocation(this->shader.Program, "texture_diffuse1"), 0);

	// draw each aircraft passed
	for (GLuint i = 0; i < toDraw.size(); i++) {
		Aircraft* current = toDraw[i];

		// Draw the loaded model
		glUniformMatrix4fv(glGetUniformLocation(this->shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(this->shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

		//apply the aircraft's current position and rotation
		// apply linear velocity to change position
		GLfloat positionChangeMagnitude = current->velocityMagnitude * timeDelta;
		current->position = current->position + current->velocityDirection * positionChangeMagnitude;
		// apply new position
		glm::mat4 model;
		model = glm::translate(model, current->position);
		// set current rotation
		model = glm::rotate(model, glm::radians(current->rotationAngle), current->rotationAxis);
		//apply angular velocity
		model = glm::rotate(model, glm::radians(current->angularVelocityMagnitude*timeValue), current->angularVelocityAxis);
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		//apply attenuation based on distance
		GLfloat distToCamSqrd = pow(camPosition.x - current->position.x, 2) + pow(camPosition.y - current->position.y, 2) + pow(camPosition.z + current->position.z, 2);
		GLfloat distanceToCam = sqrtf(distToCamSqrd);
		float attenuation = 1.0 / (1.0 + 0.00014 * distanceToCam + 0.0000007 * distToCamSqrd);
		glUniform1f(glGetUniformLocation(this->shader.Program, "attenuation"), attenuation);

		current->model.Draw(this->shader);

		//detect collision with camera:
		GLfloat collisionRadius = 40.0f; // change this to be a property of the aircraft!
		if (distanceToCam < collisionRadius && !current->hasCollided) {
			std::cout << "Collision with Aircraft detected!" << std::endl;
			current->hasCollided = true;
		}
	}

	glBindVertexArray(0); //unbind vertex array

	//unbind textures after drawing
	Texture::Unbind();

	this->previousTimeStep = timeValue;
}

AircraftDrawer::~AircraftDrawer()
{
}
