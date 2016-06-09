#include "PlaneDrawer.h"

PlaneDrawer::PlaneDrawer(Texture & tex, Shader & planeShader)
{
	this->tex = tex;
	this->shader = planeShader;
	this->previousTimeStep = 0;
}

const float PlaneDrawer::YAW_PER_ROLL = 0.5f;

void PlaneDrawer::Draw(glm::mat4 view, glm::mat4 projection, glm::vec3 camPosition, GLfloat timeValue, std::vector<Aircraft*> & toDraw)
{
	GLfloat timeDelta = timeValue - this->previousTimeStep;

	this->shader.Use();

	// load up textures
	this->tex.Use(GL_TEXTURE0);
	GLuint uniformLoc = glGetUniformLocation(this->shader.Program, "texture_diffuse1");
	glUniform1i(glGetUniformLocation(this->shader.Program, "texture_diffuse1"), 0);

	// draw each plane passed to the method
	for (GLuint i = 0; i < toDraw.size(); i++) {
		Aircraft* current = toDraw[i];

		// Draw the loaded model
		glUniformMatrix4fv(glGetUniformLocation(this->shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(this->shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		// This code changes the plane's orientation to navigate to its current waypoint
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		if (current->IsAutonomous()) {
			// get the active waypoint
			Waypoint * active = current->GetPath()->GetActiveWaypoint();
			if (active != nullptr) {
				glm::vec3 activePosition = active->GetPosition();
				//check if we are within the completion radius of the waypoint
				GLfloat distSqrd = pow(current->position.x - activePosition.x, 2) + pow(current->position.y - activePosition.y, 2) + pow(current->position.z - activePosition.z, 2);
				if (distSqrd < pow(current->GetPath()->waypointCompletionRadius, 2)) {
					current->GetPath()->CompleteWaypoint();
				}
				else {
					//horizontal navigation in the x-z plane
					glm::vec3 planeDirection;
					planeDirection.x = cos(glm::radians(current->pitch)) * cos(glm::radians(current->yaw + 90));
					planeDirection.y = 0; // restrict to horizontal plane
					planeDirection.z = cos(glm::radians(current->pitch)) * sin(glm::radians(current->yaw + 90));
					planeDirection = glm::normalize(planeDirection);

					glm::vec3 vectorToObject;
					vectorToObject.x = activePosition.x - current->position.x;
					vectorToObject.y = 0; // restrict to horizontal plane
					vectorToObject.z = activePosition.z - current->position.z;
					vectorToObject = glm::normalize(vectorToObject);

					//determine angle b/t vectors
					GLfloat dotProd = glm::dot(planeDirection, vectorToObject);
					// safety check before using acos:
					if (dotProd < -1.0) dotProd = -1.0f;
					else if (dotProd > 1.0) dotProd = 1.0f;
					GLfloat angleMagnitude = acos(dotProd);
					GLfloat angleSign = 1.0f;
					glm::vec3 normalToPlane = glm::cross(vectorToObject, planeDirection);
					if (glm::dot(glm::vec3(0.0f, 1.0f, 0.0f), normalToPlane) > 0) {
						angleSign = -1.0f;
					}
					GLfloat angle = angleSign * angleMagnitude; //angle between vectors
					angle = max(angle, -1.0f);
					angle = min(angle, 1.0f);


					// weight function goes to zero as we get closer to target line
					GLfloat sigmoid = 2 / (1 + exp(-25 * (angle))) - 1;
					GLfloat deltaAngle = -sigmoid * 50.0f - current->roll;
					GLfloat deltaAngleMag = abs(deltaAngle);
					GLfloat deltaAngleSign = 1.0f;
					if (deltaAngle < 0.0f) {
						deltaAngleSign = -1.0f;
					}
					current->roll += min(deltaAngleMag, 1.5f) * deltaAngleSign; //control speed of turn to make it smooth

					// vertical navigation - change pitch to seek active waypoint

					vectorToObject.x = activePosition.x - current->position.x;
					vectorToObject.y = activePosition.y - current->position.y;
					vectorToObject.z = activePosition.z - current->position.z;
					//create vector in th x-z plane parallel to the the vector to the object - this is essentially the projection of the above on to the horizontal plane
					glm::vec3 parallelVector;
					parallelVector.x = activePosition.x - current->position.x;
					parallelVector.y = 0;
					parallelVector.z = activePosition.z - current->position.z;

					// determine angle of elevation
					GLfloat hypotenuseLength = glm::length(vectorToObject);
					GLfloat length = glm::length(parallelVector);
					angle = acos(length / hypotenuseLength);
					angle = max(angle, 0.0f);
					angle = min(angle, 1.0f);

					sigmoid = 1 / (1 + exp(-35 * (angle - 0.2)));
					GLfloat sign = 1.0f;
					if (vectorToObject.y < 0) {
						sign = -1.0f;
					}
					deltaAngle = sign * sigmoid * 50.0f - current->pitch;
					deltaAngleMag = abs(deltaAngle);
					deltaAngleSign = 1.0f;
					if (deltaAngle < 0.0f) {
						deltaAngleSign = -1.0f;
					}
					current->pitch += min(deltaAngleMag, 0.25f) * deltaAngleSign;
				}
			}
		} // else proceed as normal


		/////////////////////////////////////////////////////////////////////////////////////////////////////
		// The following code advances the plane forward along its current orientation
		/////////////////////////////////////////////////////////////////////////////////////////////////////

		//account for roll
		// if there is roll, then yaw should be gradually changed
		GLfloat deltaYaw = PlaneDrawer::YAW_PER_ROLL * timeDelta * current->roll;
		current->yaw += deltaYaw;

		// determine aircraft direction from pitch, roll, and velocity and update position
		glm::vec3 direction;
		direction.x = cos(glm::radians(current->pitch)) * cos(glm::radians(current->yaw + 90));
		direction.y = sin(glm::radians(current->pitch));
		direction.z = cos(glm::radians(current->pitch)) * sin(glm::radians(current->yaw + 90));
		glm::vec3 unitDirection = glm::normalize(direction);
		current->position = current->position + current->speed * timeDelta * unitDirection;


		glm::mat4 model;
		// translate to current position
		model = glm::translate(model, current->position);
		// apply orientation - the order here actually seems to matter :/
		model = glm::rotate(model, glm::radians(current->yaw), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-current->pitch), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(current->roll), glm::vec3(0.0f, 0.0f, 1.0f));
		// planes need to be scaled (the normal model is massive)
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		//apply attenuation based on distance
		GLfloat distToCamSqrd = pow(camPosition.x - current->position.x, 2) + pow(camPosition.y - current->position.y, 2) + pow(camPosition.z + current->position.z, 2);
		GLfloat distanceToCam = sqrtf(distToCamSqrd);
		float attenuation = 1.0 / (1.0 + 0.00014 * distanceToCam + 0.0000007 * distToCamSqrd);
		glUniform1f(glGetUniformLocation(this->shader.Program, "attenuation"), attenuation);

		current->model.Draw(this->shader);

		//detect collision with camera:
		GLfloat collisionRadius = current->collisionRadius;
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

PlaneDrawer::~PlaneDrawer()
{
}
