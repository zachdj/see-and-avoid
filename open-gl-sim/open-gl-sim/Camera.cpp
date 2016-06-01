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
	this->SCREEN_W = screenW;
	this->SCREEN_H = screenH;

	this->previousTimeValue = 0;
	this->speed = 0.0f;
	this->position = position;
	this->direction = glm::vec3(0.0f, 0.0f, -1.0f);
	this->pitch = 0.0f;
	this->yaw = 0.0f;
	this->roll = 0.0f;
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
			newRoll -= timeDelta * this->ROLL_NG_VELOCITY;
		} else if(rollRightActive && !rollLeftActive){
			newRoll += timeDelta * this->ROLL_NG_VELOCITY;
		}
		if ( newRoll!=this->roll && newRoll <= this->MAX_ROLL && newRoll >= this->MIN_ROLL) {
			this->roll = newRoll;
		}

		//pitch
		bool pitchForwardActive = KeyboardHandler::keys[GLFW_KEY_UP];
		bool pitchBackwardActive = KeyboardHandler::keys[GLFW_KEY_DOWN];
		GLfloat newPitch = this->pitch;
		if (pitchForwardActive && !pitchBackwardActive) {
			newPitch -= timeDelta * this->PITCH_NG_VELOCITY;
		}
		else if (pitchBackwardActive && !pitchForwardActive) {
			newPitch += timeDelta * this->PITCH_NG_VELOCITY;
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
	}
	else {
		// Movement controls for a rotor
	}

	//move UAV forward along current direction if velocity != 0

	this->previousTimeValue = timeValue;
}

glm::mat4 Camera::GetCameraViewMatrix()
{
	//TODO
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
