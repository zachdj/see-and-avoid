#pragma once

#include <glew.h>; // Include glew to get all the required OpenGL headers

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <algorithm>
using namespace std;

#include "Path.h"
#include "KeyboardHandler.h"

using namespace std;

class Camera
{
	public:
		Camera();
		Camera(GLuint screenW, GLuint screenH, glm::vec3 position);
		void DoMovement(GLfloat timeValue); //should be called at the start of every frame
		void ActivateAutonomousMode();
		void DeactivateAutonomousMode();
		void SetPath(Path * path);
		Path * GetPath();
		bool IsAutonomousNavigationActive();
		glm::mat4 GetCameraViewMatrix();
		glm::mat4 GetProjectionMatrix();
		glm::mat4 GetOrthoMatrix();
		glm::vec3 GetPosition();
		glm::vec3 GetCurrentDirection();
		~Camera();

	private :
		bool autonomousModeActive;
		Path * path;
		void DoAutonomousMovement(GLfloat timeDelta);
		void DoKeyboardMovement(GLfloat timeDelta);

		//shared config variables
		GLfloat viewDistance = 3000.0f;
		GLfloat FOV = 66.0f;
		// config variables for flight
		GLfloat MAX_SPEED = 50.0f; // distance units / second
		GLfloat ACCELERATION = 10.0f; // units / second
		GLfloat MIN_SPEED;
		glm::vec3 INITIAL_POSITION;
		GLuint SCREEN_W;
		GLuint SCREEN_H;

		//fixed-wing config
		GLfloat MIN_PITCH = -90.0f; GLfloat MAX_PITCH = 90.0f;
		GLfloat MIN_ROLL = -45.0f; GLfloat MAX_ROLL = 45.0f;
		GLfloat YAW_NG_VELOCITY = 4.0f; // pure yaw without roll is pretty slow for fixed-wings I think
		GLfloat PITCH_NG_VELOCITY = 15.0f; // degrees / second
		GLfloat ROLL_NG_VELOCITY = 15.0f;
		GLfloat YAW_PER_ROLL = 0.5f; // degree of yaw per degree of roll per time

		//rotor-specific config
		GLfloat ROTOR_YAW_NG_VELOCITY = 60.0f; // yaw is much faster for rotors
		GLfloat ROTOR_ACCELERATION = 15.0f;
		GLfloat ROTOR_VERTICAL_VELOCITY = 8.0f;
		GLfloat MAX_ROTOR_PITCH = 25.0f; //degrees

		//status variables
		GLfloat previousTimeValue;
		glm::vec3 position;
		// For ease of development and debugging, all angles are in degrees.  They are converted to rads by glm::radians
		GLfloat pitch;
		GLfloat yaw;
		GLfloat roll;
		GLfloat speed;
		GLfloat rotorStrafeSpeed;

		void ResetOrientation();
		void Reset();
};