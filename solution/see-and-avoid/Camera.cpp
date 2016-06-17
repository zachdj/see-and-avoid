#include "Camera.h"

void PrintVector(glm::vec3 vector, GLchar* name) {

	std::cout << name << ": " << vector.x << ", " << vector.y << ", " << vector.z << std::endl;
}

Camera::Camera()
{
	this->SCREEN_W = 800;
	this->SCREEN_H = 800;
}

Camera::Camera(GLuint screenW, GLuint screenH, glm::vec3 position)
{
	this->SCREEN_W = screenW;
	this->SCREEN_H = screenH;

	this->autonomousModeActive = false;
	this->path = new Path(); //default path is empty

	this->INITIAL_POSITION = position;
	this->position = position;
	this->previousTimeValue = 0;
	this->pitch = 0.0f;
	this->yaw = 0.0f;
	this->roll = 0.0f;
	this->rotorStrafeSpeed = 0.0f;
}

// this function advances the camera forward either using autonomous navigation or keyboard navigation
void Camera::DoMovement(GLfloat timeValue)
{
	GLfloat timeDelta = timeValue - this->previousTimeValue;

	if (!this->autonomousModeActive) {
		this->DoKeyboardMovement(timeDelta);
	} else {
		this->DoAutonomousMovement(timeDelta);
	}

	// key sequence for resetting orientation and velocity
	if (KeyboardHandler::keys[GLFW_KEY_LEFT_CONTROL] || KeyboardHandler::keys[GLFW_KEY_RIGHT_CONTROL]) {
		this->ResetOrientation();
	} 
	// spacebar resets the whole contraption
	if (KeyboardHandler::keys[GLFW_KEY_SPACE]) {
		this->Reset();
	}

	this->previousTimeValue = timeValue;
}

void Camera::ActivateAutonomousMode()
{
	this->autonomousModeActive = true;
	this->speed = this->MAX_SPEED;
}

void Camera::DeactivateAutonomousMode()
{
	this->autonomousModeActive = false;
	this->speed = MIN_SPEED;
}

void Camera::SetPath(Path * path)
{
	this->path = path;
}

Path * Camera::GetPath()
{
	return this->path;
}

bool Camera::IsAutonomousNavigationActive()
{
	return this->autonomousModeActive;
}

glm::mat4 Camera::GetCameraViewMatrix()
{
	glm::mat4 view; //rotate camera appropriately
	view = glm::rotate(view, glm::radians(this->roll), glm::vec3(0.0f, 0.0f, -1.0f));
	view = glm::rotate(view, glm::radians(this->pitch), glm::vec3(1.0f, 0.0f, 0.0f));
	view = glm::rotate(view, glm::radians(this->yaw), glm::vec3(0.0f, 1.0f, 0.0f));
	view = glm::translate(view, this->position); //move camera to current position
	return view;

}

glm::mat4 Camera::GetProjectionMatrix()
{
	return glm::perspective(glm::radians(45.0f), (GLfloat)this->SCREEN_W / (GLfloat)this->SCREEN_H, 0.1f, this->viewDistance); // GLM handles creation of perspective transformation matrix
}

glm::mat4 Camera::GetOrthoMatrix() {
	return glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
}

glm::vec3 Camera::GetPosition(){
	return this->position; // GLM handles creation of perspective transformation matrix
}

void Camera::ResetOrientation() {
	this->pitch = 0; this->yaw = 0; this->roll = 0;
	this->speed = 0; this->rotorStrafeSpeed = 0;
}

void Camera::Reset() {
	this->ResetOrientation();
	this->position = this->INITIAL_POSITION;
}


// private methods
void Camera::DoAutonomousMovement(GLfloat timeDelta) {
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// This code changes the plane's orientation to navigate to its current waypoint
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// get the active waypoint
	Waypoint * active = this->GetPath()->GetActiveWaypoint();
	if (active != nullptr) {
		glm::vec3 activePosition = active->GetPosition();
		//check if we are within the completion radius of the waypoint
		glm::vec3 vectorToObject;
		vectorToObject.x = activePosition.x + this->position.x;
		vectorToObject.y = 0; // restrict to horizontal plane
		vectorToObject.z = activePosition.z + this->position.z;

		if (glm::length(vectorToObject) < this->GetPath()->waypointCompletionRadius) {
			this->GetPath()->CompleteWaypoint();
		}
		else {
			vectorToObject = glm::normalize(vectorToObject);

			glm::vec3 planeDirection;
			planeDirection.z = -cos(glm::radians(this->pitch)) * cos(glm::radians(this->yaw));
			planeDirection.y = 0; // restrict to horizontal plane
			planeDirection.x = cos(glm::radians(this->pitch)) * sin(glm::radians(this->yaw));
			planeDirection = glm::normalize(planeDirection);

			//find angle between the current direction and the direction to the object (direction we want to go)
			GLfloat dotProd = glm::dot(planeDirection, vectorToObject);
			// safety check before using acos;  prevents exceptions due to floating point roundoff
			if (dotProd < -1.0) dotProd = -1.0f;
			else if (dotProd > 1.0) dotProd = 1.0f;
			GLfloat angleMagnitude = acos(dotProd);
			GLfloat angleSign = 1.0f;
			glm::vec3 normalToPlane = glm::cross(vectorToObject, planeDirection);
			if (glm::dot(glm::vec3(0.0f, 1.0f, 0.0f), normalToPlane) > 0) {
				angleSign = -1.0f;
			}
			GLfloat angle = angleSign * angleMagnitude; //angle between vectors

			//we have the angle between the current orientation and the direction we need to be going.  Now map that angle to a certain degree of roll
			GLfloat weight = (1 * angleMagnitude + 1)*(1 * angleMagnitude + 1);  // (2x + 1)^2
			weight = -(weight + 1) / weight + 2; // 2 - ((2x + 1)^2 + 1) / (2x + 1)^2
			//weight function maps angle into a value between zero and 1 - this is the percentage of max roll
			GLfloat newRoll = angleSign * weight * 45.0f; // 45 degrees is max roll

			GLfloat deltaRoll = newRoll - this->roll;
			this->roll += ((deltaRoll > 0) - (deltaRoll < 0)) * min(abs(deltaRoll), 0.5f);

			// vertical navigation: as long as there's a height difference, adjust pitch to accomodate

			GLfloat deltaHeight = activePosition.y + this->position.y;
			weight = -(2 / (1 + exp(-0.05 * deltaHeight)) - 1); // logistic function between -1 and 1
			GLfloat newPitch = weight * 45.0f; // max pitch is 45 degrees
			GLfloat deltaPitch = newPitch - this->pitch;
			this->pitch += ((deltaPitch > 0) - (deltaPitch < 0)) * min(abs(deltaPitch), 0.5f);			
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// The following code advances the plane forward along its current orientation
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	//account for roll
	// if there is roll, then yaw should be gradually changed
	GLfloat deltaYaw = 0.5f * timeDelta * this->roll; //0.5 is yaw per roll rate
	this->yaw -= deltaYaw;

	// determine aircraft direction from pitch, roll, and velocity and update position
	glm::vec3 direction;
	direction.x = cos(glm::radians(this->pitch)) * cos(glm::radians(this->yaw + 90));
	direction.y = sin(glm::radians(this->pitch));
	direction.z = cos(glm::radians(this->pitch)) * sin(glm::radians(this->yaw + 90));
	glm::vec3 unitDirection = glm::normalize(direction);
	this->position = this->position + this->speed * timeDelta * unitDirection;

}

void Camera::DoKeyboardMovement(GLfloat timeDelta) {
	//set fixed-wing or rotor mode for upcoming loop
	if (KeyboardHandler::fixedWing) {
		this->MIN_SPEED = 5.0f;
	}
	else {
		this->MIN_SPEED = -this->MAX_SPEED;
	}

	if (KeyboardHandler::fixedWing) {
		// Movement controls for a fixed wing

		//roll
		bool rollLeftActive = KeyboardHandler::keys[GLFW_KEY_A];
		bool rollRightActive = KeyboardHandler::keys[GLFW_KEY_D];
		GLfloat newRoll = this->roll;
		if (rollLeftActive && !rollRightActive) {
			newRoll += timeDelta * this->ROLL_NG_VELOCITY;
		}
		else if (rollRightActive && !rollLeftActive) {
			newRoll -= timeDelta * this->ROLL_NG_VELOCITY;
		}
		if (newRoll != this->roll) {
			this->roll = newRoll;
			this->roll = std::fmin(this->roll, this->MAX_ROLL);
			this->roll = std::fmax(this->roll, this->MIN_ROLL);
		}
		// if there is roll, then yaw should be gradually changed
		GLfloat deltaYaw = -0.5 * timeDelta * this->roll;
		this->yaw += deltaYaw;

		//pitch
		bool pitchForwardActive = KeyboardHandler::keys[GLFW_KEY_W];
		bool pitchBackwardActive = KeyboardHandler::keys[GLFW_KEY_S];
		GLfloat newPitch = this->pitch;
		if (pitchForwardActive && !pitchBackwardActive) {
			newPitch += timeDelta * this->PITCH_NG_VELOCITY;
		}
		else if (pitchBackwardActive && !pitchForwardActive) {
			newPitch -= timeDelta * this->PITCH_NG_VELOCITY;
		}
		if (newPitch != this->pitch) {
			this->pitch = newPitch;
			this->pitch = std::fmin(this->pitch, this->MAX_PITCH);
			this->pitch = std::fmax(this->pitch, this->MIN_PITCH);
		}

		//yaw
		bool yawCounterCWActive = KeyboardHandler::keys[GLFW_KEY_Q];
		bool yawCWActive = KeyboardHandler::keys[GLFW_KEY_E];
		GLfloat newYaw = this->yaw;
		if (yawCounterCWActive && !yawCWActive) {
			newYaw -= timeDelta * this->YAW_NG_VELOCITY;
		}
		else if (yawCWActive && !yawCounterCWActive) {
			newYaw += timeDelta * this->YAW_NG_VELOCITY;
		}
		if (newYaw != this->yaw) {
			this->yaw = newYaw;
		}

		// acceleration
		bool accelerateActive = KeyboardHandler::keys[GLFW_KEY_UP];
		bool decelerateActive = KeyboardHandler::keys[GLFW_KEY_DOWN];
		GLfloat newSpeed = this->speed;
		if (accelerateActive && !decelerateActive) {
			newSpeed += timeDelta * this->ACCELERATION;
		}
		else if (decelerateActive && !accelerateActive) {
			newSpeed -= timeDelta * this->ACCELERATION;
		}
		//clamp speed b/t min and max values
		this->speed = newSpeed;
		this->speed = std::fmin(this->speed, this->MAX_SPEED);
		this->speed = std::fmax(this->speed, this->MIN_SPEED);


		//move UAV forward along current direction if speed > 0
		if (this->speed > 0) {
			//find direction
			glm::vec3 direction;
			direction.x = cos(glm::radians(this->pitch)) * cos(glm::radians(this->yaw + 90));
			direction.y = sin(glm::radians(this->pitch));
			direction.z = cos(glm::radians(this->pitch)) * sin(glm::radians(this->yaw + 90));
			glm::vec3 unitDirection = glm::normalize(direction);
			this->position = this->position + this->speed * timeDelta * unitDirection;
		}

	}
	else {
		// Movement controls for a rotor

		//yaw - rotors can spin, yo
		bool yawCounterCWActive = KeyboardHandler::keys[GLFW_KEY_Q];
		bool yawCWActive = KeyboardHandler::keys[GLFW_KEY_E];
		GLfloat newYaw = this->yaw;
		if (yawCounterCWActive && !yawCWActive) {
			newYaw -= timeDelta * this->ROTOR_YAW_NG_VELOCITY;
		}
		else if (yawCWActive && !yawCounterCWActive) {
			newYaw += timeDelta * this->ROTOR_YAW_NG_VELOCITY;
		}
		if (newYaw != this->yaw) {
			this->yaw = newYaw;
		}

		// acceleration
		bool accelerateActive = KeyboardHandler::keys[GLFW_KEY_UP];
		bool decelerateActive = KeyboardHandler::keys[GLFW_KEY_DOWN];
		GLfloat newSpeed = this->speed;
		if (accelerateActive && !decelerateActive) {
			newSpeed += timeDelta * this->ROTOR_ACCELERATION;
		}
		else if (decelerateActive && !accelerateActive) {
			newSpeed -= timeDelta * this->ROTOR_ACCELERATION;
		}
		//clamp speed b/t min and max values
		this->speed = newSpeed;
		this->speed = std::fmin(this->speed, this->MAX_SPEED);
		this->speed = std::fmax(this->speed, this->MIN_SPEED);

		//strafe
		bool strafeLeftActive = KeyboardHandler::keys[GLFW_KEY_A];
		bool strafeRightActive = KeyboardHandler::keys[GLFW_KEY_D];
		GLfloat newStrafe = this->rotorStrafeSpeed;
		if (strafeLeftActive && !strafeRightActive) {
			newStrafe -= this->ACCELERATION * timeDelta;
		}
		else if (strafeRightActive && !strafeLeftActive) {
			newStrafe += this->ACCELERATION * timeDelta;
		}
		if (newStrafe != this->rotorStrafeSpeed && newStrafe <= this->MAX_SPEED && newStrafe >= this->MIN_SPEED) {
			this->rotorStrafeSpeed = newStrafe;
		}

		//determine roll and pitch from strafe and forward speeds
		GLfloat speedRange = this->MAX_SPEED - this->MIN_SPEED;
		GLfloat pct = -this->rotorStrafeSpeed / speedRange;
		this->roll = pct * this->MAX_ROTOR_PITCH;
		pct = this->speed / speedRange;
		this->pitch = pct * this->MAX_ROTOR_PITCH;

		// find  current direction - this is different for rotors, because they don't work like a Euler-angle gimbal
		// We treat pitch and roll like they're always zero
		glm::vec3 direction;
		direction.x = cos(glm::radians(this->yaw + 90));
		direction.y = 0.0f;
		direction.z = sin(glm::radians(this->yaw + 90));
		glm::vec3 unitDirection = glm::normalize(direction);
		if (this->speed != 0) {
			this->position = this->position + this->speed * timeDelta * unitDirection;
		}
		if (this->rotorStrafeSpeed != 0) {
			// cross product of current heading with the vertial will give a positive vector normal to the forward direction
			glm::vec3 strafeDirection = glm::cross(direction, glm::vec3(0.0f, 1.0f, 0.0f));
			this->position = this->position + this->rotorStrafeSpeed * timeDelta * strafeDirection;
		}
		// move vertically
		if (KeyboardHandler::keys[GLFW_KEY_W]) {
			this->position = this->position - this->ROTOR_VERTICAL_VELOCITY * timeDelta * glm::vec3(0.0f, 1.0f, 0.0f);
		}
		else if (KeyboardHandler::keys[GLFW_KEY_S]) {
			this->position = this->position + this->ROTOR_VERTICAL_VELOCITY * timeDelta * glm::vec3(0.0f, 1.0f, 0.0f);
		}
	}

}


Camera::~Camera()
{
}
