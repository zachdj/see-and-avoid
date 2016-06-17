#pragma once

#define _ITERATOR_DEBUG_LEVEL 0
#define PI 3.14159265
#define DEBUG false
#define RANDOM true

//OpenCV includes
#include <opencv2/highgui/highgui.hpp>
#include <opencv2\opencv.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdlib.h>     

using namespace cv;

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

#include "KeyboardHandler.h"
#include "Skybox.h"
#include "Shader.h"
#include "Camera.h"
#include "Cube.h"
#include "CubeDrawer.h"
#include "Aircraft.h"
#include "PlaneDrawer.h"

#include "BlobTracker.h"

using namespace std;

vector< Aircraft*> generatePlanes();
vector< Aircraft*> generateRandomPlanes();

vector< Mat> planePaths;
vector< Aircraft*> myplanes;

int planeSelection = 0;
Camera camera;

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

void MyLine(Mat img, Point start, Point end, int red, int green, int blue)
{
	int thickness = 3;
	int lineType = 8;
	line(img,
		Point((start.x+1000)/4, (start.y + 1000) /4),
		Point((end.x + 1000) / 4, (end.y + 1000) / 4),
		Scalar(red, green, blue),
		thickness,
		lineType);
}

float point2pointDistance2(int pt1x, int pt1y, int pt2x, int pt2y) {
	return  pow((pt1x - pt2x), 2) + pow((pt1y - pt2y), 2);
}

float point2pointAngle(int pt1x, int pt1y, int pt2x, int pt2y) {
	if (pt1x - pt2x != 0)
		return  atan((pt1y - pt2y) / (pt1x - pt2x));
	else
		return atan((pt1y - pt2y) / (0.000001));
}

void on_trackbar(int, void*);

//forward declarations go here

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
	GLFWwindow* window = glfwCreateWindow(width/2.0, height/2.0, "See and Avoid Sim", nullptr, nullptr);
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
	
	//setup the key callback
	glfwSetKeyCallback(window, key_callback);

	//set the clear color
	glClearColor(0.8f, 0.9f, 1.0f, 1.0f); // sky blue-ish;  We should never see this if the skybox is working

	//tell openGL to use depth testing
	glEnable(GL_DEPTH_TEST);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // uncomment to enable Wireframe mode
	
	Shader skyboxShader(".\\Shaders\\Skybox\\skybox.vs", ".\\Shaders\\Skybox\\skybox.fs");
	Skybox * skybox = new Skybox(skyboxShader);

	//create some cubes!
	//create a CubeDrawer
	Shader cubeShader(".\\Shaders\\Cube\\cube.vs", ".\\Shaders\\Cube\\cube.fs");
	Texture woodBoxTexture(".\\asset\\container.jpg");
	Texture acmeTexture(".\\asset\\acme.jpg");
	CubeDrawer * cubeDrawer = new CubeDrawer(woodBoxTexture, acmeTexture, cubeShader);

	// create a plane that follows a path
	Shader planeShader(".\\Shaders\\Aircraft\\aircraft.vs", ".\\Shaders\\Aircraft\\aircraft.fs");

	if (RANDOM) {
		namedWindow("Plane Paths", CV_WINDOW_AUTOSIZE);
		cv::moveWindow("Plane Paths", 400, 10);
		myplanes = generateRandomPlanes();
		createTrackbar("Plane Select: ", "Plane Paths", &planeSelection, planePaths.size()-1, on_trackbar);
		on_trackbar(1, 0);
		//imshow("Plane Paths", planePaths.at(planeSelection));
	}
	else
		myplanes = generatePlanes();

	PlaneDrawer * planeDrawer = new PlaneDrawer(woodBoxTexture, planeShader);
	
	// create a path for the camera (our plane)
	//initialize Camera

	camera = Camera(width, height, glm::vec3(0.0f, 0.0f, -1000.0f));
	vector<Waypoint *> cameraWaypoints;
	cameraWaypoints.push_back(new Waypoint(glm::vec3(0.0f, 0.0f, -500.0f)));
	cameraWaypoints.push_back(new Waypoint(glm::vec3(400.0f, 0.0f, -900.0f)));
	cameraWaypoints.push_back(new Waypoint(glm::vec3(300.0f, 0.0f, -1400.0f)));
	cameraWaypoints.push_back(new Waypoint(glm::vec3(0.0f, 0.0f, -1000.0f)));
	cameraWaypoints.push_back(new Waypoint(glm::vec3(0.0f, 0.0f, -500.0f)));
	cameraWaypoints.push_back(new Waypoint(glm::vec3(0.0f, 0.0f, 500.0f)));
	cameraWaypoints.push_back(new Waypoint(glm::vec3(-400.0f, 0.0f, 900.0f)));
	cameraWaypoints.push_back(new Waypoint(glm::vec3(-300.0f, 0.0f, 1400.0f)));
	cameraWaypoints.push_back(new Waypoint(glm::vec3(0.0f, 0.0f, 1000.0f)));
	cameraWaypoints.push_back(new Waypoint(glm::vec3(0.0f, 0.0f, 500.0f)));
	Path * cameraPath = new Path(cameraWaypoints, 20.0f);
	camera.SetPath(cameraPath);
	camera.ActivateAutonomousMode();
	//cameraPath->SetAvoidanceWaypoint(new Waypoint(glm::vec3(0.0f, 10.0f, 750.0f)));

	//create a cube at each waypoint for debugging purposes
	vector< Cube> myCubes; //cubes to be drawn
	if (DEBUG) {
		myCubes.push_back( Cube(glm::vec3(0.0f, 0.0f, -500.0f)));
		myCubes.push_back( Cube(glm::vec3(400.0f, 0.0f, -900.0f)));
		myCubes.push_back( Cube(glm::vec3(300.0f, 0.0f, -1400.0f)));
		myCubes.push_back( Cube(glm::vec3(0.0f, 0.0f, -1000.0f)));
		myCubes.push_back( Cube(glm::vec3(0.0f, 0.0f, -500.0f)));
		myCubes.push_back( Cube(glm::vec3(0.0f, 0.0f, 500.0f)));
		myCubes.push_back( Cube(glm::vec3(-400.0f, 0.0f, 900.0f)));
		myCubes.push_back( Cube(glm::vec3(-300.0f, 0.0f, 1400.0f)));
		myCubes.push_back( Cube(glm::vec3(0.0f, 0.0f, 1000.0f)));
		myCubes.push_back( Cube(glm::vec3(0.0f, 0.0f, 500.0f)));
	}





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
		skybox->Draw(view, projection);

		cubeDrawer->Draw(view, projection, timeValue, myCubes);

		//draw aircraft
		planeDrawer->Draw(view, projection, camera.GetPosition(), timeValue, myplanes);

		// swap buffers to display what we just rendered on the back buffer
		glfwSwapBuffers(window);

		on_trackbar(1, 0);
	}


	glfwTerminate();
	return 0;
}


vector< Aircraft*> generatePlanes() {
	vector< Aircraft*> myLocalplanes;
	vector<Waypoint *> waypoints;


	//plane 1
	waypoints.push_back(new Waypoint(glm::vec3(0.0f, 0.0f, 500.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(-400.0f, 0.0f, 900.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(-300.0f, 0.0f, 1400.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(0.0f, 0.0f, 1000.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(0.0f, 0.0f, 500.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(0.0f, 0.0f, -500.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(400.0f, 0.0f, -900.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(300.0f, 0.0f, -1400.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(0.0f, 0.0f, -1000.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(0.0f, 0.0f, -500.0f)));
	Path planePath = Path(waypoints, 20.0f);
	Aircraft* plane = new Aircraft(glm::vec3(0.0f, 0.0f, -700.0f), planePath, 40.0f, ".\\Models\\fighter\\fighter.obj", 0.1);
	plane->SetSpeed(50.0f);
	myLocalplanes.push_back(plane);

	//plane 2
	waypoints.clear();
	waypoints.push_back(new Waypoint(glm::vec3(0.0f, 0.0f, 500.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(-400.0f, 0.0f, 70.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(-300.0f, 0.0f, 400.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(400.0f, 0.0f, -500.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(300.0f, 0.0f, -40.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(0.0f, 0.0f, -700.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(0.0f, 0.0f, -500.0f)));
	Path planePath2 = Path(waypoints, 20.0f);
	Aircraft* plane2 = new Aircraft(glm::vec3(0.0f, 0.0f, -100.0f), planePath2, 40.0f, ".\\Models\\plane\\plane.obj", 0.7);
	plane2->SetSpeed(100.0f);
	myLocalplanes.push_back(plane2);

	//plane 3
	waypoints.clear();
	waypoints.push_back(new Waypoint(glm::vec3(0.0f, 0.0f, 0.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(0.0f, 0.0f, -1000.0f)));
	//waypoints.push_back(new Waypoint(glm::vec3(-300.0f, 0.0f, 1400.0f)));
	//waypoints.push_back(new Waypoint(glm::vec3(400.0f, 0.0f, -90.0f)));
	//waypoints.push_back(new Waypoint(glm::vec3(300.0f, 0.0f, -1400.0f)));
	//waypoints.push_back(new Waypoint(glm::vec3(20.0f, 0.0f, -700.0f)));
	//waypoints.push_back(new Waypoint(glm::vec3(0.0f, 0.0f, 520.0f)));
	Path planePath3 = Path(waypoints, 20.0f);
	Aircraft* plane3 = new Aircraft(glm::vec3(0.0f, 0.0f, -200.0f), planePath3, 40.0f, ".\\Models\\vought\\vought.obj", 0.01);
	plane3->SetSpeed(50.0f);
	plane3->SetOrientation(0.0f, 180.0f, 0.0f);
	myLocalplanes.push_back(plane3);

	//plane 4
	waypoints.clear();
	waypoints.push_back(new Waypoint(glm::vec3(400.0f, 0.0f, 50.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(-900.0f, 0.0f, 90.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(-300.0f, 0.0f, 800.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(400.0f, 0.0f, -900.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(350.0f, 0.0f, -20.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(-300.0f, 0.0f, -10.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(0.0f, 0.0f, -500.0f)));
	Path planePath4 = Path(waypoints, 20.0f);
	Aircraft* plane4 = new Aircraft(glm::vec3(0.0f, 0.0f, -400.0f), planePath4, 40.0f, ".\\Models\\plane\\plane.obj", 0.7);
	plane4->SetSpeed(20.0f);
	myLocalplanes.push_back(plane4);

	//plane 5
	waypoints.clear();
	waypoints.push_back(new Waypoint(glm::vec3(0.0f, 0.0f, 500.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(-400.0f, 0.0f, 900.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(-300.0f, 0.0f, 400.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(400.0f, 0.0f, -900.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(400.0f, 0.0f, -900.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(300.0f, 0.0f, -400.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(0.0f, 0.0f, -1000.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(0.0f, 0.0f, 500.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(-400.0f, 0.0f, 900.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(20.0f, 0.0f, -100.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(0.0f, 0.0f, -500.0f)));
	Path planePath5 = Path(waypoints, 20.0f);
	Aircraft* plane5 = new Aircraft(glm::vec3(0.0f, 0.0f, -1000.0f), planePath5, 40.0f, ".\\Models\\plane\\plane.obj", 0.7);
	plane5->SetSpeed(100.0f);
	myLocalplanes.push_back(plane5);

	return myLocalplanes;

}

vector< Aircraft*> generateRandomPlanes() {
	vector< Aircraft*> myLocalplanes;
	vector<Waypoint *> waypoints;
	vector<Point> points;
	vector<vector<Point>> planePoints;


	cout << "Enter the number of random generated planes..." << endl;
	int numberOfPlanes;
	cin >> numberOfPlanes;

	int widthOfAirspace = 2000;
	int waypointSize = 4;

	//As many planes as you want
	for (int i = 0; i < numberOfPlanes; i++) {
		waypoints.clear();
		for (int j = 0; j < waypointSize; j++) {
			float x = (float)(rand() % widthOfAirspace - widthOfAirspace/2);
			float z = (float)(rand() % widthOfAirspace - widthOfAirspace / 2);
			waypoints.push_back(new Waypoint(glm::vec3(x, 0.0f, z)));
			points.push_back(Point(x, z));
		}
		Path planePath = Path(waypoints, 20.0f);
		int planeType = rand() % 3;
		Aircraft* plane;
		if (planeType == 0)
			plane = new Aircraft(glm::vec3((float)(rand() % widthOfAirspace - widthOfAirspace / 2), 0.0f, (float)(rand() % widthOfAirspace - widthOfAirspace / 2)), planePath, 40.0f, ".\\Models\\vought\\vought.obj", 0.01);
		else if (planeType == 1)
			plane = new Aircraft(glm::vec3((float)(rand() % widthOfAirspace - widthOfAirspace / 2), 0.0f, (float)(rand() % widthOfAirspace - widthOfAirspace / 2)), planePath, 40.0f, ".\\Models\\fighter\\fighter.obj", 0.1);
		else
			plane = new Aircraft(glm::vec3((float)(rand() % widthOfAirspace - widthOfAirspace / 2), 0.0f, (float)(rand() % widthOfAirspace - widthOfAirspace / 2)), planePath, 40.0f, ".\\Models\\plane\\plane.obj", 0.7);
		plane->SetSpeed(50.0f);
		myLocalplanes.push_back(plane);
		planePoints.push_back(points);
		points.clear();
	}

	//fix this piece of shit Matrix
	Mat completeDrawing(500, 500, CV_8UC3, Scalar(255, 255, 255));
	//As many planes as you want
	for (int i = 0; i < numberOfPlanes; i++) {
		Mat individual(500, 500, CV_8UC3, Scalar(255, 255, 255));
		int red = rand() % 255; int green = rand() % 255; int blue = rand() % 255;
		for (int j = 1; j <= waypointSize; j++) {
			if (j != waypointSize) {
				MyLine(completeDrawing, planePoints.at(i).at(j), planePoints.at(i).at(j - 1), red, green, blue);
				MyLine(individual, planePoints.at(i).at(j), planePoints.at(i).at(j - 1), red, green, blue);
			}
			else {
				MyLine(completeDrawing, planePoints.at(i).at(j - 1), planePoints.at(i).at(0), red, green, blue);
				MyLine(individual, planePoints.at(i).at(j - 1), planePoints.at(i).at(0), red, green, blue);
			}
		}
		planePaths.push_back(individual);
	}
	planePaths.push_back(completeDrawing);

	return myLocalplanes;

}

void on_trackbar(int, void*) {
	Mat local = planePaths.at(planeSelection).clone();
	if (planeSelection == myplanes.size()) {
		for (int i = 0; i < myplanes.size(); i++) {
			circle(local, Point((myplanes.at(i)->position.x + 1000) / 4, (myplanes.at(i)->position.z + 1000) / 4), 10, Scalar(0, 0, 0), 3);
		}
	}
	else {
		circle(local, Point((myplanes.at(planeSelection)->position.x + 1000) / 4, (myplanes.at(planeSelection)->position.z + 1000) / 4), 10, Scalar(0, 0, 0), 3);
	}

	circle(local, Point(-(camera.GetPosition().x - 1000) / 4, -(camera.GetPosition().z - 1000) / 4), 10, Scalar(0,0,255), 3);
	imshow("Plane Paths", local);
	
}