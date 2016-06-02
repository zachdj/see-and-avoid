#include "KeyboardHandler.h"
#include <iostream>
using namespace std;

bool KeyboardHandler::keys[1024];

bool KeyboardHandler::fixedWing = true;

void KeyboardHandler::handle_key_press(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS){
		keys[key] = true;
	} else if(action == GLFW_RELEASE){
		keys[key] = false;
	}

	//special case: F key switches between fixed-wing and rotor modes
	if (action == GLFW_RELEASE && key == GLFW_KEY_F) {
		KeyboardHandler::fixedWing = !KeyboardHandler::fixedWing;
	}

}

