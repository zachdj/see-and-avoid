#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include <glew.h>; // Include glew to get all the required OpenGL headers

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "KeyboardHandler.h"

class Camera
{
	public:
		Camera();
		Camera(GLuint screenW, GLuint screenH, bool fixed_wing_mode, glm::vec3 position);
		void DoMovement(GLfloat timeValue); //should be called at the start of every frame
		glm::mat4 GetCameraViewMatrix();
		glm::mat4 GetProjectionMatrix();
		glm::mat4 GetOrthoMatrix();
		~Camera();

	private :
		GLfloat MAX_SPEED = 50.0f; // distance units / second
		GLfloat MIN_PITCH = -90.0f; GLfloat MAX_PITCH = 90.0f;
		GLfloat MIN_ROLL = -45.0f; GLfloat MAX_ROLL = 45.0f;
		GLfloat PITCH_NG_VELOCITY = 15.0f; // degrees / second
		GLfloat ROLL_NG_VELOCITY = 15.0f;
		GLfloat YAW_NG_VELOCITY = 4.0f; // pure yaw without roll is pretty slow for fixed-wings I think
		GLfloat ROTOR_YAW_NG_VELOCITY = 60.0f; // yaw is much faster for rotors
		GLfloat ACCELERATION = 10.0f; // units / second
		GLfloat ROTOR_ACCELERATION = 15.0f;
		GLfloat ROTOR_VERTICAL_VELOCITY = 8.0f;
		GLfloat MAX_ROTOR_PITCH = 25.0f; //degrees
		bool FIXED_WING_MODE;
		GLfloat MIN_SPEED;
		glm::vec3 INITIAL_POSITION;
		GLuint SCREEN_W;
		GLuint SCREEN_H;

		GLfloat previousTimeValue;
		glm::vec3 position;
		GLfloat speed;
		GLfloat rotorStrafeSpeed;
		glm::vec3 direction;
		// For ease of development and debugging, all angles are in degrees.  They are converted to rads by glm::radians
		GLfloat pitch;
		GLfloat yaw;
		GLfloat roll;
};

#endif

