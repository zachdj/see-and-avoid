
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>

#include <iostream>
#include <cstdio>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "KeyboardHandler.h"

#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Cube.h"
#include "CubeDrawer.h"

using namespace std;

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

//responds to keyboard events
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	KeyboardHandler::handle_key_press(window, key, scancode, action, mods);	
}

int main() {
	//set error CB function
	glfwSetErrorCallback(error_callback);
	
	//try to intialize GLFW
	if (glfwInit() != GL_TRUE) {
		exit(EXIT_FAILURE);
	}
	//GLFW initialized now.  Set window hints for OpenGL v3.3 and make window fixed size
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // this will cause openGL to raise an error when we try to call legacy library functions
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // no resizing for you, Mr. End-User

	//Time to create a window with GLFW
	GLFWwindow* window = glfwCreateWindow(1600, 900, "See and Avoid Sim", nullptr, nullptr);
	if (window == nullptr) //ensure the window was initialized
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//Initialize GLEW before calling any openGL functions
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	//get viewport dimensions from GLFW and initialize OpenGL
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	//initialize Camera
	Camera camera(width, height, true, glm::vec3(0.0f, 0.0f, -10.0f));
	
	//setup the key callback
	glfwSetKeyCallback(window, key_callback);

	//set the clear color
	glClearColor(0.8f, 0.9f, 1.0f, 1.0f); // sky blue or close

	//tell openGL to use depth testing
	glEnable(GL_DEPTH_TEST);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		
	

	//create a cube!
	Cube myCube1(glm::vec3(0.0f, 0.0f, 5.0f));
	Cube myCube2(glm::vec3(-2.0f, 1.0f, 0.0f));
	Cube myCube3(glm::vec3(0.0f, 0.0f, 0.0f));
	Cube myCube4(glm::vec3(2.0f, -1.0f, 0.0f));
	Cube myCube5(glm::vec3(4.0f, 1.0f, 0.0f));
	Cube myCube6(glm::vec3(3.0f, 0.0f, 0.0f));
	//myCube.SetVelocity(10.0f, glm::vec3(0.0f, 0.0f, 4.0f));
	myCube1.SetAngularVelocity(12.0f, glm::vec3(1.0f, 0.5f, 0.2f));
	//Cube toDraw[] = { myCube1, myCube2, myCube3, myCube4, myCube5, myCube6};
	Cube toDraw[] = { myCube1 };
	//create a CubeDrawer
	Shader defaultShader("Shaders/Cube/cube.vs", "Shaders/Cube/cube.fs");
	Texture woodBoxTexture("../asset/container.jpg");
	Texture acmeTexture("../asset/acme.jpg");
	CubeDrawer drawer(camera, woodBoxTexture, acmeTexture, defaultShader);
	
	//render, event, and frame buffer loop
	while (!glfwWindowShouldClose(window))
	{
		// poll for events
		glfwPollEvents();

		//render stuff and things
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the screen - will be filled with clear color set above

		//the current time
		GLfloat timeValue = glfwGetTime();

		//maybe combine these two calls
		drawer.camera.DoMovement(timeValue);
		drawer.Draw(timeValue, toDraw, 1);

		// swap buffers to display what we just rendered to the back buffer
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}