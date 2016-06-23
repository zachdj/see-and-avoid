#pragma once

#define _ITERATOR_DEBUG_LEVEL 0
#define PI 3.14159265
#define DEBUG false

#include <thread>
#include <mutex>

#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>

#include <iostream>
#include <cstdio>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <sstream>

#include "KeyboardHandler.h"
#include "Skybox.h"
#include "Shader.h"
#include "Camera.h"
#include "Aircraft.h"
#include "PlaneDrawer.h"
#include "PathHelper.h"
#include "PlaneGenerator.h"
#include "PrintToFile.h"
#include "Algorithms\AtcAvoidance.h"
#include "VisionProcessor.h"

using namespace std;

/**************************** forward declarations go here **************************************************/

bool RANDOM = false;

// functions to run in threads
int renderScene();
int processScene();
mutex semaphore;
cv::Mat img; // this will contain the current view rendered by openGL.  Must be locked before any R/W operations
bool renderingStopped = false;

// callback functions
static void error_callback(int error, const char* description);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void on_trackbar(int, void*);

//variables for top-down plane tracker
vector< Mat> planePaths;
vector<Mat> PlanePathMatrices;
int planeSelection = 0;
int widthOfAirspace = 4000;

vector< Aircraft*> myplanes; // planes to render
Camera camera; // camera object
AtcAvoidance ai = AtcAvoidance(); // avoidance algorithm to use

/***************************** End forward declarations ********************************************************/

int main() {

	PrintToFile::clearFile();
	PrintToFile::clearDebugFile();	

	thread renderThread(renderScene);

	thread processThread(processScene);

	if (renderThread.joinable()) {
		renderThread.join();
	}
	if (processThread.joinable()) {
		processThread.join();
	}

	glfwTerminate();
	return 0;
}

int renderScene() {
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
	GLFWwindow* window = glfwCreateWindow(width / 2.0, height / 2.0, "See and Avoid Sim", nullptr, nullptr);
	
	if (window == nullptr) //ensure the window was initialized
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//hide the window for setup
	glfwHideWindow(window);
	glfwMakeContextCurrent(window);

	//Initialize GLEW before calling any openGL functions
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	//setup the key callback
	glfwSetKeyCallback(window, key_callback);

	//set the clear color
	glClearColor(0.8f, 0.9f, 1.0f, 1.0f); // sky blue-ish;  We should never see this if the skybox is working

	//tell openGL to use depth testing
	glEnable(GL_DEPTH_TEST);

	Shader skyboxShader(".\\Shaders\\Skybox\\skybox.vs", ".\\Shaders\\Skybox\\skybox.fs");
	Skybox * skybox = new Skybox(skyboxShader);

	// PathHelper for preloaded paths
	PathHelper * pathHelper = new PathHelper();

	// create a plane that follows a path
	Shader planeShader(".\\Shaders\\Aircraft\\aircraft.vs", ".\\Shaders\\Aircraft\\aircraft.fs");
	
	//Create Planes Before Drawing any new windows
	PlaneGenerator planeGenerator(RANDOM, widthOfAirspace);
    myplanes = planeGenerator.getPlanes();
	

	// we have to create openCV windows in this thread!
	namedWindow("Blob Detection", CV_WINDOW_NORMAL);
	cv::moveWindow("Blob Detection", 600, 10);

	PlanePathMatrices = planeGenerator.getPlanePaths();
	namedWindow("Plane Paths", CV_WINDOW_AUTOSIZE);
	cv::moveWindow("Plane Paths", 400, 200);
	createTrackbar("Plane Select: ", "Plane Paths", &planeSelection, planeGenerator.getPlanePaths().size() - 1, on_trackbar);	

	Texture defaultPlaneTexture(".\\asset\\container.jpg");
	PlaneDrawer * planeDrawer = new PlaneDrawer(defaultPlaneTexture, planeShader);

	// create camera and path for camera (our plane)
	camera = Camera(width, height, glm::vec3(0.0f, 0.0f, 1000.0f));
	camera.SetPath(pathHelper->GetPreloadedPath(0));
	camera.ActivateAutonomousMode();
	//camera.GetPath()->SetAvoidanceWaypoint(new Waypoint(glm::vec3(0.0f, 100.0f, -2000.0f)));

	//Show the Window again once we are ready
	glfwShowWindow(window);

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
		skybox->Draw(view, projection);

		//draw aircraft
		planeDrawer->Draw(camera, camera.GetPosition(), timeValue, myplanes);

		//OpenCV stuf
		//create an empty matrix to hold frame data
		semaphore.lock();
		img = cv::Mat(height / 2.0, width / 2.0, CV_8UC3);
		//use fast 4-byte alignment if possible
		glPixelStorei(GL_PACK_ALIGNMENT, (img.step & 3) ? 1 : 4);
		//set length of one complete row in destination data (doesn't need to equal img.cols)
		glPixelStorei(GL_PACK_ROW_LENGTH, img.step / img.elemSize());
		glReadPixels(0, 0, img.cols, img.rows, GL_BGR, GL_UNSIGNED_BYTE, img.data);
		cv::flip(img, img, 0);
		semaphore.unlock();

		on_trackbar(1, 0);
		
		// swap buffers to display what we just rendered on the back buffer
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	renderingStopped = true;
	return 0;
}

int processScene() {
	Mat frame;
	VisionProcessor processor = VisionProcessor();
	while (!renderingStopped) {

		semaphore.lock();
		frame = img;
		semaphore.unlock();
		vector<BlobInfo> blobs = processor.ProcessScene(frame);
		for (int i = 0; i < blobs.size(); i++) {
			ai.reactToBlob(blobs[i], camera);
		}		
	}
	return 0;
}


void on_trackbar(int, void*) {
	Mat local = PlanePathMatrices.at(planeSelection).clone();
	if (planeSelection == myplanes.size()) {
		for (int i = 0; i < myplanes.size(); i++) {
			stringstream value; value << i;
			putText(local, value.str(), Point((myplanes.at(i)->position.x + widthOfAirspace / 2) / (widthOfAirspace / 500), (myplanes.at(i)->position.z + widthOfAirspace / 2) / (widthOfAirspace / 500)), FONT_HERSHEY_SIMPLEX,0.7, Scalar(0, 0, 0),2,8,false);
		}
	}
	else {
		stringstream value; value << planeSelection;
		putText(local, value.str(), Point((myplanes.at(planeSelection)->position.x + widthOfAirspace / 2) / (widthOfAirspace / 500), (myplanes.at(planeSelection)->position.z + widthOfAirspace / 2) / (widthOfAirspace / 500)), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 0), 2, 8, false);
		line(local, Point((myplanes.at(planeSelection)->position.x + widthOfAirspace / 2) / (widthOfAirspace / 500), (myplanes.at(planeSelection)->position.z + widthOfAirspace / 2) / (widthOfAirspace / 500)), Point((myplanes.at(planeSelection)->GetPath()->GetActiveWaypoint()->GetPosition().x + widthOfAirspace / 2) / (widthOfAirspace / 500), (myplanes.at(planeSelection)->GetPath()->GetActiveWaypoint()->GetPosition().z + widthOfAirspace / 2) / (widthOfAirspace / 500)),Scalar(255,0,0),1, CV_AA);
	}
	circle(local, Point((camera.GetPosition().x + widthOfAirspace / 2) / (widthOfAirspace / 500), (camera.GetPosition().z + widthOfAirspace / 2) / (widthOfAirspace / 500)), 10, Scalar(0, 0, 255), -1);
	if (camera.GetPath() != nullptr && camera.GetPath()->GetActiveWaypoint() != nullptr) {
		circle(local, Point((camera.GetPath()->GetNextPathWaypoint()->GetPosition().x + widthOfAirspace / 2) / (widthOfAirspace / 500), (camera.GetPath()->GetNextPathWaypoint()->GetPosition().z + widthOfAirspace / 2) / (widthOfAirspace / 500)), 10, Scalar(180, 105, 255), -1);
		if(camera.GetPath()->GetActiveWaypoint()->GetPosition() != camera.GetPath()->GetNextPathWaypoint()->GetPosition())
			circle(local, Point((camera.GetPath()->GetActiveWaypoint()->GetPosition().x + widthOfAirspace / 2) / (widthOfAirspace / 500), (camera.GetPath()->GetActiveWaypoint()->GetPosition().z + widthOfAirspace / 2) / (widthOfAirspace / 500)), 7, Scalar(180, 105, 255), -1);
	
	}	
	imshow("Plane Paths", local);

}


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