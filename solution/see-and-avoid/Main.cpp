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
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "KeyboardHandler.h"
#include "Skybox.h"
#include "Shader.h"
#include "Camera.h"
#include "Aircraft.h"
#include "PlaneDrawer.h"
#include "PathHelper.h"
#include "PlaneGenerator.h"
#include "PrintToFile.h"
#include "Algorithms\DoNothingAvoidance.h"
#include "Algorithms\AtcAvoidance.h"
#include "Algorithms\AvoidanceDistanceAgnostic.h"
#include "Algorithms\AvoidanceWithDistance.h"
#include "Algorithms\AvoidanceAgnosticElevation.h"
#include "VisionProcessor.h"
#include "ContourVisionProcessor.h"

//TODO: remove
#include "Cube.h"
#include "CubeDrawer.h"
#include "Utility.h"

using namespace std;

/**************************** forward declarations go here **************************************************/
//Determines if planes are randomly generated
bool RANDOM = false;

// functions to run in threads
int renderScene();
int processScene();
mutex semaphore;
cv::Mat img; // this will contain the current view rendered by openGL.  Must be locked before any R/W operations
bool renderingStopped = false;

// callback functions
static void error_callback(int error, const char* description);
void drawAirplane(Mat & drawer, Camera camera, Aircraft * myplane, Scalar color, bool isCameraObject); 
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void on_trackbar(int, void*);

//variables for top-down plane tracker
vector< Mat> planePaths;
vector<Mat> PlanePathMatrices;
int planeSelection = 0;
int widthOfAirspace = 2000;

vector< Aircraft*> myplanes; // planes to render
Camera camera; // camera object
//DoNothingAvoidance ai = DoNothingAvoidance();
//AvoidanceDistanceAgnostic ai = AvoidanceDistanceAgnostic();
AvoidanceWithDistance ai = AvoidanceWithDistance();

/***************************** End forward declarations ********************************************************/

int main() {

	PrintToFile::clearFile();
	PrintToFile::clearDebugFile();	
	//clear the folder holding collision photos - this will have to change on *nix systems
	system("del /Q \"collisions\"");

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
	PathHelper * pathHelper = new PathHelper(widthOfAirspace / 4000.0f);

	// create a plane that follows a path
	Shader planeShader(".\\Shaders\\Aircraft\\aircraft.vs", ".\\Shaders\\Aircraft\\aircraft.fs");
	
	//Create Planes Before Drawing any new windows
	//PlaneGenerator planeGenerator(RANDOM, widthOfAirspace);
	PlaneGenerator::generateAirportPlanes(widthOfAirspace);
    myplanes = PlaneGenerator::getPlanes();	

	// we have to create openCV windows in this thread!

	namedWindow("Blob Detection", CV_WINDOW_NORMAL);
	cv::resizeWindow("Blob Detection", width/2.0, height/2.0);
	cv::moveWindow("Blob Detection", width/2.0, 0);

	PlanePathMatrices = PlaneGenerator::getPlanePaths();
	namedWindow("Plane Paths", CV_WINDOW_AUTOSIZE);
	cv::moveWindow("Plane Paths", width/2.0, height/2.0);
	createTrackbar("Plane Select: ", "Plane Paths", &planeSelection, PlaneGenerator::getPlanePaths().size() - 1, on_trackbar);	

	Texture defaultPlaneTexture(".\\asset\\container.jpg");
	PlaneDrawer * planeDrawer = new PlaneDrawer(defaultPlaneTexture, planeShader);

	// create camera and path for camera (our plane)
	float scale = widthOfAirspace / 4000.0; // 4000 was default width of airspace
	camera = Camera(width, height, scale*glm::vec3(0.0f, 0.0f, 50.0f));
	camera.SetPath(pathHelper->GetAirportPath());
	camera.ActivateAutonomousMode();
	//camera.GetPath()->SetAvoidanceWaypoint(new Waypoint(glm::vec3(-100.0f, 0.0f, -1100.0f)));

	//TODO: remove
	Cube distCube1 = Cube(glm::vec3(200.0f, -15.0f, 20.0f));
	Cube distCube2 = Cube(glm::vec3(-200.0f, -15.0f, 20.0f));
	vector<Cube> cubesToDraw = vector<Cube>();
	cubesToDraw.push_back(distCube1);
	cubesToDraw.push_back(distCube2);

	Shader cubeShader = Shader(".\\Shaders\\Cube\\cube.vs", ".\\Shaders\\Cube\\Cube.fs");
	CubeDrawer * cubeDrawer = new CubeDrawer(defaultPlaneTexture, defaultPlaneTexture, cubeShader);

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

		//TODO: remove
		//cubeDrawer->Draw(view, projection, timeValue, cubesToDraw);

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
		resize(img, img, Size(960, 540));

		//check if planedrawer has recently detected a collision and export the current openGL view to a file
		if (planeDrawer->recentCollisionTime != nullptr) {
			stringstream filename; tm* ltm = planeDrawer->recentCollisionTime;
			filename << "collisions/" << ltm->tm_hour << "_" << ltm->tm_min << "_" << ltm->tm_sec + 1 << ".jpg";
			imwrite(filename.str(), img);
			planeDrawer->recentCollisionTime = nullptr;
		}
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
	ContourVisionProcessor processor = ContourVisionProcessor();
	double focalLengthSum = 0;
	int focalLengthMeasurements = 0;
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
	if (planeSelection == myplanes.size()) { //this is for the master matrix where we place all of our planes on a single matrix
		for (int i = 0; i < myplanes.size(); i++) {
			stringstream value; value << i;
			putText(local, value.str(), Point((myplanes.at(i)->position.x + widthOfAirspace / 2) / (widthOfAirspace / 500)-3, (myplanes.at(i)->position.z + widthOfAirspace / 2) / (widthOfAirspace / 500)+5), FONT_HERSHEY_SIMPLEX,0.7, Scalar(0, 0, 0),2,8,false);
		}
	}
	else { //this works for our single airplane matrix
		stringstream value; value << planeSelection;
		drawAirplane(local, camera, myplanes.at(planeSelection), Scalar(255, 0, 0), false);  //We will now place a plane on our path
		line(local, Point((myplanes.at(planeSelection)->position.x + widthOfAirspace / 2) / (widthOfAirspace / 500), (myplanes.at(planeSelection)->position.z + widthOfAirspace / 2) / (widthOfAirspace / 500)), Point((myplanes.at(planeSelection)->GetPath()->GetActiveWaypoint()->GetPosition().x + widthOfAirspace / 2) / (widthOfAirspace / 500), (myplanes.at(planeSelection)->GetPath()->GetActiveWaypoint()->GetPosition().z + widthOfAirspace / 2) / (widthOfAirspace / 500)),Scalar(255,0,0),1, CV_AA);
	}
	// we will now draw ourselves on the matrix, and our associated waypoints
	drawAirplane(local, camera, nullptr, Scalar(0, 0, 0), true);
	if (camera.GetPath() != nullptr && camera.GetPath()->GetActiveWaypoint() != nullptr) {
		//This will draw our waypoint
		rectangle(local, Point((camera.GetPath()->GetNextPathWaypoint()->GetPosition().x + widthOfAirspace / 2) / (widthOfAirspace / 500), (camera.GetPath()->GetNextPathWaypoint()->GetPosition().z + widthOfAirspace / 2) / (widthOfAirspace / 500) - 15), Point((camera.GetPath()->GetNextPathWaypoint()->GetPosition().x + widthOfAirspace / 2) / (widthOfAirspace / 500) + 20, (camera.GetPath()->GetNextPathWaypoint()->GetPosition().z + widthOfAirspace / 2) / (widthOfAirspace / 500) - 25), Scalar(180, 105, 255), -1);
		line(local, Point((camera.GetPath()->GetNextPathWaypoint()->GetPosition().x + widthOfAirspace / 2) / (widthOfAirspace / 500), (camera.GetPath()->GetNextPathWaypoint()->GetPosition().z + widthOfAirspace / 2) / (widthOfAirspace / 500)), Point((camera.GetPath()->GetNextPathWaypoint()->GetPosition().x + widthOfAirspace / 2) / (widthOfAirspace / 500), (camera.GetPath()->GetNextPathWaypoint()->GetPosition().z + widthOfAirspace / 2) / (widthOfAirspace / 500)-25), Scalar(0, 0, 0), 3);
		//This will draw our temporary waypoint if we have one
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

void drawAirplane(Mat & drawer, Camera camera,Aircraft* myplane, Scalar color, bool isCameraObject) {

	if (isCameraObject) {
		float cameraX = (camera.GetPosition().x + widthOfAirspace / 2) / (widthOfAirspace / 500);
		float cameraZ = (camera.GetPosition().z + widthOfAirspace / 2) / (widthOfAirspace / 500);
		glm::vec3 upVec = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 orthogonalVec = glm::cross(upVec, camera.GetCurrentDirection());
		line(drawer, Point(cameraX + 10 * camera.GetCurrentDirection().x, cameraZ + 10 * camera.GetCurrentDirection().z), Point(cameraX - 18 * camera.GetCurrentDirection().x, cameraZ - 18 * camera.GetCurrentDirection().z), color, 6);
		line(drawer, Point(cameraX + 12 * orthogonalVec.x, cameraZ + 12 * orthogonalVec.z), Point(cameraX - 12 * orthogonalVec.x, cameraZ - 12 * orthogonalVec.z), color, 8);
		line(drawer, Point(cameraX - 18 * camera.GetCurrentDirection().x + 6 * orthogonalVec.x, cameraZ - 18 * camera.GetCurrentDirection().z + 6 * orthogonalVec.z), Point(cameraX - 18 * camera.GetCurrentDirection().x - 6 * orthogonalVec.x, cameraZ - 18 * camera.GetCurrentDirection().z - 6 * orthogonalVec.z), color, 4);
	}
	else {
		float airplaneX = (myplane->position.x + widthOfAirspace / 2) / (widthOfAirspace / 500);
		float airplaneZ = (myplane->position.z + widthOfAirspace / 2) / (widthOfAirspace / 500);
		glm::vec3 upVec = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 orthogonalVec = glm::cross(upVec, myplane->GetCurrentDirection());
		line(drawer, Point(airplaneX + 10 * myplane->GetCurrentDirection().x, airplaneZ + 10 * myplane->GetCurrentDirection().z), Point(airplaneX - 18 * myplane->GetCurrentDirection().x, airplaneZ - 18 * myplane->GetCurrentDirection().z), color, 6);
		line(drawer, Point(airplaneX + 12 * orthogonalVec.x, airplaneZ + 12 * orthogonalVec.z), Point(airplaneX - 12 * orthogonalVec.x, airplaneZ - 12 * orthogonalVec.z), color, 8);
		line(drawer, Point(airplaneX - 18 * myplane->GetCurrentDirection().x + 6 * orthogonalVec.x, airplaneZ - 18 * myplane->GetCurrentDirection().z + 6 * orthogonalVec.z), Point(airplaneX - 18 * myplane->GetCurrentDirection().x - 6 * orthogonalVec.x, airplaneZ - 18 * myplane->GetCurrentDirection().z - 6 * orthogonalVec.z), color, 4);
	}
}