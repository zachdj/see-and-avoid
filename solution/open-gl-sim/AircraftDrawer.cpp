#include "AircraftDrawer.h"

AircraftDrawer::AircraftDrawer(Texture & tex, Shader & aircraftShader)
{
	this->tex = tex;
	this->shader = aircraftShader;
}

void AircraftDrawer::Draw(glm::mat4 view, glm::mat4 projection, GLfloat timeValue, std::vector<Aircraft>& toDraw)
{
	// draw each aircraft passed
	for (GLuint i = 0; i < toDraw.size(); i++) {
		Aircraft current = toDraw[i];

		this->shader.Use();

		// load up textures
		this->tex.Use(GL_TEXTURE0);
		GLuint uniformLoc = glGetUniformLocation(this->shader.Program, "texture_diffuse1");
		glUniform1i(glGetUniformLocation(this->shader.Program, "texture_diffuse1"), 0);

		// Draw the loaded model
		glUniformMatrix4fv(glGetUniformLocation(this->shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(this->shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

		//apply the aircraft's current position and rotation
		glm::mat4 model;
		model = glm::translate(model, current.position);
		// apply linear velocity
		GLfloat positionChangeMagnitude = current.velocityMagnitude * timeValue;
		model = glm::translate(model, current.velocityDirection * positionChangeMagnitude);
		// set current rotation
		model = glm::rotate(model, glm::radians(current.rotationAngle), current.rotationAxis);
		//apply angular velocity
		model = glm::rotate(model, glm::radians(current.angularVelocityMagnitude*timeValue), current.angularVelocityAxis);

		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		current.model.Draw(this->shader);

		glBindVertexArray(0); //unbind vertex array

		//unbind textures after drawing
		Texture::Unbind();
	}
}

AircraftDrawer::~AircraftDrawer()
{
}
