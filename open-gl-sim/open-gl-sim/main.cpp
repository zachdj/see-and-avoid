
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>

#include "Shader.h"

#include <iostream>
#include <cstdio>

using namespace std;

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// handle WSAD arrow keys
	if ((key == GLFW_KEY_A || key == GLFW_KEY_LEFT) && action == GLFW_PRESS) {
		fprintf(stdout, "LEFT Pressed\n");
	}
	else if ((key == GLFW_KEY_D || key == GLFW_KEY_RIGHT) && action == GLFW_PRESS) {
		fprintf(stdout, "RIGHT Pressed\n");
	}
	else if ((key == GLFW_KEY_W || key == GLFW_KEY_UP) && action == GLFW_PRESS) {
		fprintf(stdout, "UP Pressed\n");
	}
	else if ((key == GLFW_KEY_S || key == GLFW_KEY_DOWN) && action == GLFW_PRESS) {
		fprintf(stdout, "DOWN Pressed\n");
	}
	else if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		fprintf(stdout, "Quitting");
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
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
	GLFWwindow* window = glfwCreateWindow(700, 600, "See and Avoid Sim", nullptr, nullptr);
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

	//get viewport dimensions from GLFW and initialize the OpenGL
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	
	//setup the key callback
	glfwSetKeyCallback(window, key_callback);

	//set the clear color
	glClearColor(0.90f, 0.90f, 0.85f, 1.0f); //off white-ish

	////glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// default program
	Shader defaultShader("default.vs", "default.fs");

	Shader simonSaysShader("simon_says.vs", "simon_says.fs");

	//draw a Simon-Says diamond
	GLfloat vertices[] = {
		-1.0f, 0.0f, 0.0f, //left
		1.0f, 0.0f, 0.0f, //right
		0.0f, 1.0f, 0.0f, //up
		0.0f, -1.0f, 0.0f, //down
		0.0f, 0.0f, 0.0f //middle
	};
	GLuint indices[] = {
		0, 4, 2,
		1, 4, 2,
		0, 4, 3,
		1, 4, 3
	};

	GLuint VBO, VAO, EBO;
	glGenBuffers(1, &VBO); 
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO); //bind vertex array
	// bind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//bind EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//set vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind array buffer
	glBindVertexArray(0); //unbind vertex array	

	//render, event, and frame buffer loop
	while (!glfwWindowShouldClose(window))
	{
		// poll for events
		glfwPollEvents();

		//render stuff and things
		glClear(GL_COLOR_BUFFER_BIT); // clear the screen - will be filled with clear color set above

		//draw triangle1
		simonSaysShader.Use();
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

		// swap buffers to display what we just rendered to the back buffer
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}