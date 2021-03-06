#include "PlaneDrawer.h"

PlaneDrawer::PlaneDrawer(Texture & tex, Shader & planeShader)
{
	this->tex = tex;
	this->shader = planeShader;
	this->previousTimeStep = 0;
	this->frontCollisionCount = 0;
	this->backCollisionCount = 0;
}

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
					GLfloat weight = 2 / (1 + exp(-10 * (angle))) - 1;
					GLfloat deltaAngle = -weight * MAX_ROLL - current->roll;
					current->roll += ((deltaAngle > 0)-(deltaAngle < 0)) * min(abs(deltaAngle), 1.5f); //control speed of turn to make it smooth

					// vertical navigation: as long as there's a height difference, adjust pitch to accomodate
					GLfloat deltaHeight = activePosition.y - current->position.y;
					weight = (2 / (1 + exp(-0.1 * deltaHeight)) - 1); // logistic function between -1 and 1
					GLfloat newPitch = weight * MAX_PITCH; // max pitch is 45 degrees
					GLfloat deltaPitch = newPitch - current->pitch;
					current->pitch += ((deltaPitch > 0) - (deltaPitch < 0)) * min(abs(deltaPitch), 0.5f);
				}
			}
		} // else proceed as normal


		/////////////////////////////////////////////////////////////////////////////////////////////////////
		// The following code advances the plane forward along its current orientation
		/////////////////////////////////////////////////////////////////////////////////////////////////////

		// determine how much plane should turn based on banking angle and speed
		// rate of turn in deg/second is 1091 * tan(roll) / TAS
		// 0.592484 is conversion factor from ft/s to knots
		if (current->speed != 0) {
			GLfloat rateOfTurn = 1091 * tan(glm::radians(current->roll)) / (current->speed * 0.592484);
			//sort of a hacky check to limit the turning rate at low speeds:
			if (abs(rateOfTurn) > 40) {
				rateOfTurn = 40 * ((rateOfTurn > 0) - (rateOfTurn < 0));
			}
			GLfloat deltaYaw = rateOfTurn * timeDelta;
			current->yaw += deltaYaw;
		}

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
			if (theta > 4.2 || theta < 2.1) { // a direct collision is pi. So, 60 degrees both ways would be pi + pi/3 and pi - pi/3 TOTAL = 120 degree collision hit
				backCollisionCount++;
				stringstream stream;
				stream << endl << "Hit in the back #" << backCollisionCount << endl;
				stream << "X: " << current->position.x << " Y: " << current->position.y << " Z: " << current->position.z << endl;
				stream << "Plane: " << i << endl; 
				time_t now = time(0);
				tm *ltm = localtime(&now);
				stream << "Time: " << ltm->tm_hour << ":" << ltm->tm_min << ":" << 1 + ltm->tm_sec << endl << endl;				
				PrintToFile::printDebug(stream.str());
				current->hasCollided = true;
			}				
			else {
				frontCollisionCount++;
				stringstream stream; 
				stream << endl << "Front collision # " << frontCollisionCount << endl;
				stream << "Location of obstacle-> X: " << current->position.x << " Y: " << current->position.y << " Z: " << current->position.z << endl;
				stream << "Location of our aircraft-> X: " << camera.GetPosition().x << " Y: " << camera.GetPosition().y << " Z: " << camera.GetPosition().z << endl;
				stream << "Distance to Cam: " << distanceToCam << " Collision Radius: " << collisionRadius << endl;
				stream << "Plane: " << i << endl;
				time_t now = time(0);
				tm *ltm = localtime(&now);
				stream << "Time: " << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec + 1 << endl;
				PrintToFile::print(stream.str());
				current->hasCollided = true;
				this->recentCollisionTime = ltm;
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
