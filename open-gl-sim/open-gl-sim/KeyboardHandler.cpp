#include "KeyboardHandler.h"
#include <iostream>
using namespace std;

bool KeyboardHandler::keys[1024];

void KeyboardHandler::handle_key_press(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS){
		keys[key] = true;
	} else if(action == GLFW_RELEASE){
		keys[key] = false;
	}
}

