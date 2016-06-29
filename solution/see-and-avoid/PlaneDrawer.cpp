#include "PlaneDrawer.h"

PlaneDrawer::PlaneDrawer(Texture & tex, Shader & planeShader)
{
	this->tex = tex;
	this->shader = planeShader;
	this->previousTimeStep = 0;

}

const float PlaneDrawer::YAW_PER_ROLL = 0.5f;

void PlaneDrawer::Draw(Camera camera, glm::vec3 camPosition, GLfloat timeValue, std::vector<Aircraft*> & toDraw)
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
		glUniformMatrix4fv(glGetUniformLocation(this->shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(camera.GetProjectionMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(this->shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetCameraViewMatrix()));

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		// This code changes the plane's orientation to navigate to its current waypoint
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		if (current->IsAutonomous()) {
			// get the active waypoint
			Waypoint * active = current->GetPath()->GetActiveWaypoint();
			if (active != nullptr) {
				glm::vec3 activePosition = active->GetPosition();

				glm::vec3 vectorToObject;
				vectorToObject.x = activePosition.x - current->position.x;
				vectorToObject.y = 0; // restrict to horizontal plane
				vectorToObject.z = activePosition.z - current->position.z;
				//check if we are within the completion radius of the waypoint
				if (glm::length(vectorToObject) < current->GetPath()->waypointCompletionRadius) {
					current->GetPath()->CompleteWaypoint();
				}
				else {
					//horizontal navigation in the x-z plane
					glm::vec3 planeDirection;
					planeDirection.x = cos(glm::radians(current->pitch)) * cos(glm::radians(current->yaw + 90));
					planeDirection.y = 0; // restrict to horizontal plane
					planeDirection.z = cos(glm::radians(current->pitch)) * sin(glm::radians(current->yaw + 90));
					planeDirection = glm::normalize(planeDirection);

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

					// weight function goes to zero as we get closer to target line
					GLfloat weight = 2 / (1 + exp(-25 * (angle))) - 1;
					GLfloat deltaAngle = -weight * 50.0f - current->roll;
					current->roll += ((deltaAngle > 0)-(deltaAngle < 0)) * min(abs(deltaAngle), 1.5f); //control speed of turn to make it smooth

					// vertical navigation: as long as there's a height difference, adjust pitch to accomodate
					GLfloat deltaHeight = activePosition.y - current->position.y;
					weight = (2 / (1 + exp(-0.1 * deltaHeight)) - 1); // logistic function between -1 and 1
					GLfloat newPitch = weight * 45.0f; // max pitch is 45 degrees
					GLfloat deltaPitch = newPitch - current->pitch;
					current->pitch += ((deltaPitch > 0) - (deltaPitch < 0)) * min(abs(deltaPitch), 0.5f);
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
		glm::vec3 unitDirection = current->GetCurrentDirection();
		current->position = current->position + current->speed * timeDelta * unitDirection;


		glm::mat4 model;
		// translate to current position
		model = glm::translate(model, current->position);
		// apply orientation - the order here actually seems to matter :/
		model = glm::rotate(model, glm::radians(current->yaw), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-current->pitch), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(current->roll), glm::vec3(0.0f, 0.0f, 1.0f));
		// planes need to be scaled (the normal model is massive)
		model = glm::scale(model, glm::vec3(current->scale, current->scale, current->scale));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		//apply attenuation based on distance
		GLfloat distToCamSqrd = pow(camPosition.x - current->position.x, 2) + pow(camPosition.y - current->position.y, 2) + pow(camPosition.z - current->position.z, 2);
		GLfloat distanceToCam = sqrtf(distToCamSqrd);
		float attenuation = 1.0 / (1.0 + 0.00014 * distanceToCam + 0.0000007 * distToCamSqrd);
		glUniform1f(glGetUniformLocation(this->shader.Program, "attenuation"), attenuation);

		current->model.Draw(this->shader);

		
		//detect collision with camera:
		GLfloat collisionRadius = current->collisionRadius;

		if (distanceToCam < collisionRadius && !current->hasCollided) {
			glm::vec3 planeDir = current->GetCurrentDirection();
			glm::vec3 cameraDir = camera.GetCurrentDirection();
			double dotProduct = glm::dot(planeDir, cameraDir);
			float theta = acos(dotProduct); //The denominator here is one because we are using two unit vectors
			//cout << theta << endl;
			if (theta > 4.7 || theta < 1.6) { // a direct collision is pi. So, 90 degrees both ways would be pi + pi/2 and pi - pi/2
				PrintToFile::printDebug("Hit in the back",true);
				stringstream x, y, z;
				x << current->position.x; y << current->position.y; z << current->position.z;
				PrintToFile::printDebug("X: " + x.str() + " Y: " + y.str() + " Z:" + z.str());
				stringstream planeNum; planeNum << i;
				PrintToFile::printDebug("Plane: " + planeNum.str());
				//Get the current time as well
				time_t now = time(0);
				tm *ltm = localtime(&now);
				stringstream hour, min, sec; hour << ltm->tm_hour; min << ltm->tm_min; sec << 1 + ltm->tm_sec;
				PrintToFile::printDebug("Time: " + hour.str() + ":" + min.str() + ":" + sec.str());
				PrintToFile::printDebug("");
				current->hasCollided = true;
			}
				
			else {
				//std::cout << "Collision with Aircraft detected!" << std::endl;
				//PrintToFile::print("Collision with Aircraft detected!");
				stringstream x, y, z; 
				x << current->position.x; y << current->position.y; z << current->position.z;
				PrintToFile::print("X: " + x.str() + " Y: " + y.str() + " Z:" + z.str(),true);
				stringstream planeNum; planeNum << i;
				PrintToFile::print("Plane: " + planeNum.str());
				//Get the current time as well
				time_t now = time(0);
				tm *ltm = localtime(&now);
				stringstream hour, min, sec; hour << ltm->tm_hour; min << ltm->tm_min; sec << 1 + ltm->tm_sec;
				PrintToFile::print("Time: " + hour.str() + ":" + min.str() + ":" + sec.str());
				PrintToFile::print("");
				current->hasCollided = true;
			}
			
		}
		else if (distanceToCam < collisionRadius && current->hasCollided) {
			current->hasCollided = true;
		}
		else
			current->hasCollided = false;
	}

	glBindVertexArray(0); //unbind vertex array

	//unbind textures after drawing
	Texture::Unbind();

	this->previousTimeStep = timeValue;
}

PlaneDrawer::~PlaneDrawer()
{
}
