#include "Camera.h"

#include <iostream>
using namespace std;

Camera::Camera()
{
	this->SCREEN_W = 800;
	this->SCREEN_H = 800;
}

Camera::Camera(GLuint screenW, GLuint screenH, bool fixed_wing, glm::vec3 position)
{
	this->FIXED_WING_MODE = fixed_wing;
	if (fixed_wing) {
		this->MIN_SPEED = 5.0f;
		this->speed = this->MIN_SPEED;
	} else {
		this->MIN_SPEED = -this->MAX_SPEED;
		this->speed = 0;
	}

	this->SCREEN_W = screenW;
	this->SCREEN_H = screenH;

	this->INITIAL_POSITION = position;
	this->position = position;
	this->previousTimeValue = 0;
	this->direction = glm::vec3(0.0f, 0.0f, -1.0f);
	this->pitch = 0.0f;
	this->yaw = 0.0f;
	this->roll = 0.0f;
	this->rotorStrafeSpeed = 0.0f;
}

// this functio
void Camera::DoMovement(GLfloat timeValue)
{
	GLfloat timeDelta = timeValue - this->previousTimeValue;
	if (this->FIXED_WING_MODE) {
		// Movement controls for a fixed wing

		//roll
		bool rollLeftActive = KeyboardHandler::keys[GLFW_KEY_A];
		bool rollRightActive = KeyboardHandler::keys[GLFW_KEY_D];
		GLfloat newRoll = this->roll;
		if (rollLeftActive && !rollRightActive) {
			newRoll += timeDelta * this->ROLL_NG_VELOCITY;
		} else if(rollRightActive && !rollLeftActive){
			newRoll -= timeDelta * this->ROLL_NG_VELOCITY;
		}
		if ( newRoll!=this->roll && newRoll <= this->MAX_ROLL && newRoll >= this->MIN_ROLL) {
			this->roll = newRoll;
		}
		// if there is roll, then yaw should be gradually changed
		GLfloat deltaYaw = -2.5 * timeDelta * this->roll;
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
		if (newPitch != this->pitch && newPitch <= this->MAX_PITCH && newPitch >= this->MIN_PITCH) {
			this->pitch = newPitch;
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
		} else if(KeyboardHandler::keys[GLFW_KEY_S]){
			this->position = this->position + this->ROTOR_VERTICAL_VELOCITY * timeDelta * glm::vec3(0.0f, 1.0f, 0.0f);
		}		
	}


	// key sequence for resetting orientation and velocity
	if (KeyboardHandler::keys[GLFW_KEY_LEFT_CONTROL] || KeyboardHandler::keys[GLFW_KEY_RIGHT_CONTROL]) {
		this->pitch = 0; this->yaw = 0; this->roll = 0;
		this->speed = 0; this->rotorStrafeSpeed = 0;
		this->direction = glm::vec3(0.0f, 0.0f, -1.0f);
	} 
	// spacebar resets the whole contraption
	if (KeyboardHandler::keys[GLFW_KEY_SPACE]) {
		this->pitch = 0; this->yaw = 0; this->roll = 0;
		this->speed = 0; this->rotorStrafeSpeed = 0;
		this->direction = glm::vec3(0.0f, 0.0f, -1.0f);
		this->position = this->INITIAL_POSITION;
	}

	this->previousTimeValue = timeValue;
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
	return glm::perspective(glm::radians(45.0f), (GLfloat)this->SCREEN_W / (GLfloat)this->SCREEN_H, 0.1f, 1000.0f); // GLM handles creation of perspective transformation matrix
}

glm::mat4 Camera::GetOrthoMatrix() {
	return glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
}


Camera::~Camera()
{
}
