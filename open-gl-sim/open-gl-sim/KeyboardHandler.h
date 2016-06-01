#pragma once
#ifndef KEYBOARDHANDLER_H
#define KEYBOARDHANDLER_H

#include <glew.h>
#include <glfw3.h>

class KeyboardHandler
{
public:
	static void handle_key_press(GLFWwindow* window, int key, int scancode, int action, int mods);

	static bool keys[1024];

};

#endif

