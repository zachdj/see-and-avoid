#pragma once

#define _ITERATOR_DEBUG_LEVEL 0
#include <opencv2\opencv.hpp>
using namespace cv;

#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>

#include <iostream>
#include <cstdio>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "KeyboardHandler.h"
#include "Skybox.h"
#include "Shader.h"
#include "Camera.h"
#include "Cube.h"
#include "CubeDrawer.h"
#include "Aircraft.h"
#include "PlaneDrawer.h"

using namespace std;

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

//responds to keyboard events
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	KeyboardHandler::handle_key_press(window, key, scancode, action, mods);
	//handle escape key
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
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

	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	GLuint width = mode->width;
	GLuint height = mode->height;

	//Time to create a window with GLFW
	GLFWwindow* window = glfwCreateWindow(width/1.2, height/1.2, "See and Avoid Sim", nullptr, nullptr);
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

	//initialize Camera
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, -10.0f));

	//setup the key callback
	glfwSetKeyCallback(window, key_callback);

	//set the clear color
	glClearColor(0.8f, 0.9f, 1.0f, 1.0f); // sky blue-ish;  We should never see this if the skybox is working

	//tell openGL to use depth testing
	glEnable(GL_DEPTH_TEST);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // uncomment to enable Wireframe mode
	
	Shader skyboxShader(".\\Shaders\\Skybox\\skybox.vs", ".\\Shaders\\Skybox\\skybox.fs");
	Skybox skybox = Skybox(skyboxShader);

	//create some cubes!
	vector<Cube> myCubes;
	//cube tunnel!
	/*for (int i = 0; i < 250; i++) {
		Cube newCube1(glm::vec3(5.0f * sin(i), 5.0 * cos(i) , -10.0f * i));
		Cube newCube2(glm::vec3(-5.0f * sin(i), -5.0 * cos(i) , -10.0f * i));
		Cube newCube3(glm::vec3(5.0f * sin(i + 3.14159 / 2), 5.0f * cos(i + 3.14159 / 2) , -10.0f * i));
		Cube newCube4(glm::vec3(-5.0 * sin(i + 3.14159*2.5), -5.0f * cos(i + 3.14159*2.5) , -10.0f * i));
		myCubes.push_back(newCube1);
		myCubes.push_back(newCube2);
		myCubes.push_back(newCube3);
		myCubes.push_back(newCube4);
	}*/

	//create a CubeDrawer
	Shader defaultShader(".\\Shaders\\Cube\\cube.vs", ".\\Shaders\\Cube\\cube.fs");
	Texture woodBoxTexture(".\\asset\\container.jpg");
	Texture acmeTexture(".\\asset\\acme.jpg");
	CubeDrawer drawer(woodBoxTexture, acmeTexture, defaultShader);

	// create a planeDrawer
	Shader planeShader(".\\Shaders\\Aircraft\\aircraft.vs", ".\\Shaders\\Aircraft\\aircraft.fs");
	vector< Aircraft*> myplanes;
	vector<Waypoint *> waypoints;
	waypoints.push_back(new Waypoint(glm::vec3(0.0f, 0.0f, -500.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(300.0f, 150.0f, -800.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(0.0f, 0.0f, -1200.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(-300.0f, -100.0f, -800.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(-150.0f, 50.0f, -200.0f)));
	Path myPath = Path(waypoints, 20.0f);
	Aircraft* plane = new Aircraft(glm::vec3(0.0f, 0.0f, -25.0f), myPath, 40.0f, ".\\Models\\plane\\plane.obj");
	plane->SetSpeed(50.0f);
	plane->SetOrientation(0.0f, 180.0f, 0.0f);
	myplanes.push_back(plane);
	PlaneDrawer planeDrawer(woodBoxTexture, planeShader);

	//create a cube at each waypoint
	myCubes.push_back( Cube(glm::vec3(0.0f, 0.0f, -500.0f)));
	myCubes.push_back( Cube(glm::vec3(300.0f, 150.0f, -800.0f)));
	myCubes.push_back( Cube(glm::vec3(0.0f, 0.0f, -1200.0f)));
	myCubes.push_back( Cube(glm::vec3(-300.0f, -100.0f, -800.0f)));
	myCubes.push_back( Cube(glm::vec3(-150.0f, 50.0f, -200.0f)));

	//openCV camera display
	VideoCapture cap(0); // open the default camera
	if (!cap.isOpened()) {
		return -1;
	}
	Mat edges;
	namedWindow("edges", 0);
	resizeWindow("edges", width / 2.0, height / 2.0);

	//render, event, and frame buffer loop
	while (!glfwWindowShouldClose(window))
	{
		// poll for events
		glfwPollEvents();
		// clear screen and render clear color (this should be covered by skybox)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the screen - will be filled with clear color set above
		// the current time in seconds
		GLfloat timeValue = glfwGetTime();

		camera.DoMovement(timeValue);

		glm::mat4 projection, view;

		// draw other (non-skybox) objects below
		view = camera.GetCameraViewMatrix();
		projection = camera.GetProjectionMatrix();

		//draw skybox
		skybox.Draw(view, projection);

		drawer.Draw(view, projection, timeValue, myCubes);

		//draw aircraft
		planeDrawer.Draw(view, projection, camera.GetPosition(), timeValue, myplanes);

		//openCV stuff

		//create an empty matrix to hold frame data
		cv::Mat img(height/2.0, width/2.0, CV_8UC3);
		//use fast 4-byte alignment if possible
		glPixelStorei(GL_PACK_ALIGNMENT, (img.step & 3) ? 1 : 4);
		//set length of one complete row in destination data (doesn't need to equal img.cols)
		glPixelStorei(GL_PACK_ROW_LENGTH, img.step / img.elemSize()); 
		glReadPixels(0, 0, img.cols, img.rows, GL_BGR, GL_UNSIGNED_BYTE, img.data);
		cv::flip(img, img, 0);
		//cvtColor(img, img, CV_BGR2GRAY);
		//GaussianBlur(img, img, Size(7, 7), 1.5, 1.5);
		Canny(img, img, 180, 180, 3);

		imshow("edges", img);
		//if (waitKey(30) >= 0) break;

		// swap buffers to display what we just rendered on the back buffer
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}