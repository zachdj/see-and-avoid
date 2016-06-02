
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>

#include <iostream>
#include <cstdio>
#include <algorithm>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SOIL.h>

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

// Loads a cubemap texture from 6 individual texture faces
// Order should be:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)
GLuint loadCubemap(vector<const GLchar*> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
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
	
	GLfloat skyboxVertices[] = {
		// Positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	// Setup skybox VAO
	GLuint skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);

	// Cubemap (Skybox)
	vector<const GLchar*> faces;
	faces.push_back("skybox/skybox2/right.png");
	faces.push_back("skybox/skybox2/left.png");
	faces.push_back("skybox/skybox2/top.png");
	faces.push_back("skybox/skybox2/bottom.png");
	faces.push_back("skybox/skybox2/back.png");
	faces.push_back("skybox/skybox2/front.png");
	GLuint cubemapTexture = loadCubemap(faces);

	Shader skyboxShader("Shaders/Skybox/skybox.vs", "Shaders/Skybox/skybox.fs");
	

	//create a cube!
	Cube myCube1(glm::vec3(0.0f, 0.0f, -15.0f));
	Cube myCube2(glm::vec3(-2.0f, 1.0f, 0.0f));
	Cube myCube3(glm::vec3(0.0f, 0.0f, 0.0f));
	Cube myCube4(glm::vec3(2.0f, -1.0f, 0.0f));
	Cube myCube5(glm::vec3(4.0f, 1.0f, 0.0f));
	Cube myCube6(glm::vec3(3.0f, 0.0f, 0.0f));
	//myCube.SetVelocity(10.0f, glm::vec3(0.0f, 0.0f, 4.0f));
	//myCube1.SetAngularVelocity(12.0f, glm::vec3(1.0f, 0.5f, 0.2f));
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

		//clear screen and reander clear color (this should be covered by skybox)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the screen - will be filled with clear color set above

		//the current time
		GLfloat timeValue = glfwGetTime();

		//maybe combine these two calls
		camera.DoMovement(timeValue);

		//render skybox
		glDepthMask(GL_FALSE);
		skyboxShader.Use();
		// Set view and projection matrix
		glm::mat4 view = glm::mat4(glm::mat3(camera.GetCameraViewMatrix()));	// Remove any translation component of the view matrix
		//glm::mat4 view = camera.GetCameraViewMatrix();
		glm::mat4 projection = camera.GetProjectionMatrix();
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		//draw skybox first
		glBindVertexArray(skyboxVAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthMask(GL_TRUE);

		// draw other (non-skybox) objects below
		drawer.Draw(timeValue, toDraw, 1);

		// swap buffers to display what we just rendered to the back buffer
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}