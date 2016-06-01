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
		GLfloat MAX_SPEED = 15.0f;
		GLfloat MIN_PITCH = -90.0f; GLfloat MAX_PITCH = 90.0f;
		GLfloat MIN_ROLL = -45.0f; GLfloat MAX_ROLL = 45.0f;
		GLfloat PITCH_NG_VELOCITY = 15.0f; // degrees / second
		GLfloat ROLL_NG_VELOCITY = 15.0f;
		GLfloat YAW_NG_VELOCITY = 15.0f;
		bool FIXED_WING_MODE;
		GLuint SCREEN_W;
		GLuint SCREEN_H;

		GLfloat previousTimeValue;
		glm::vec3 position;
		GLfloat speed;
		glm::vec3 direction;
		// For ease of development and debugging, all angles are in degrees.  They are converted to rads by glm::radians
		GLfloat pitch;
		GLfloat yaw;
		GLfloat roll;
};

#endif

