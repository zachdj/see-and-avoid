#pragma once

#include <glew.h>
#include <glfw3.h>

class KeyboardHandler
{
public:
	static void handle_key_press(GLFWwindow* window, int key, int scancode, int action, int mods);

	static bool keys[1024];

	static bool fixedWing;  // will become true when the "switch planes" key is pressed.  The camera can read this value to see if it needs to switch modes

};

