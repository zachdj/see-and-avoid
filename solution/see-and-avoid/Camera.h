
/*
The camera is actually our own plane. Though we do not have a direct size of our aircraft, the camera is the object used to move
within the test environment. We will claim this to be our own aircraft.
*/

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
#include "PrintToFile.h"

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
		bool IsTurningLeft();
		bool IsTurningRight();
		glm::mat4 GetCameraViewMatrix();
		glm::mat4 GetProjectionMatrix();
		glm::mat4 GetOrthoMatrix();
		glm::vec3 GetPosition();
		GLfloat GetSpeed();
		glm::vec3 GetCurrentDirection();
		glm::vec3 GetCurrentDirectionFlat();
		~Camera();

	private :
		bool autonomousModeActive;
		Path * path;
		void DoAutonomousMovement(GLfloat timeDelta);
		void DoKeyboardMovement(GLfloat timeDelta);

		int waypointsCompleted = 1;

		//shared config variables
		GLfloat viewDistance = 8000.0f;
		GLfloat FOV = 40.0f; // chosen to minimize perspective distortion
		// config variables for flight
		GLfloat MAX_SPEED = 50.0f; // distance units / second
		GLfloat ACCELERATION = 10.0f; // units / second
		GLfloat MIN_SPEED;
		glm::vec3 INITIAL_POSITION;
		GLuint SCREEN_W;
		GLuint SCREEN_H;

		//fixed-wing config
		GLfloat MIN_PITCH = -60.0f; GLfloat MAX_PITCH = 45.0f;
		GLfloat MIN_ROLL = -45.0f; GLfloat MAX_ROLL = 45.0f;
		GLfloat YAW_NG_VELOCITY = 4.0f; // pure yaw without roll is pretty slow for fixed-wings I think
		GLfloat PITCH_NG_VELOCITY = 15.0f; // degrees / second
		GLfloat ROLL_NG_VELOCITY = 25.0f;

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