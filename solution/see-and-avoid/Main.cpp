#pragma once

#define _ITERATOR_DEBUG_LEVEL 0
#define PI 3.14159265
#define DEBUG false

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
#include "PathHelper.h"
#include "PlaneGenerator.h"
#include "PrintToFile.h"
#include <math.h>

#include <sstream>
#include "BlobTracker.h"

using namespace std;

/**************************** forward declarations go here **************************************************/

bool RANDOM = false;

int renderScene();
int processScene();

static void error_callback(int error, const char* description);

void MyLine(Mat img, Point start, Point end, int red, int green, int blue);

//responds to keyboard events
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);


float point2pointDistance2(int pt1x, int pt1y, int pt2x, int pt2y);
float point2pointAngle(int pt1x, int pt1y, int pt2x, int pt2y);
void on_trackbar(int, void*);


mutex semaphore;
cv::Mat img; // this will contain the current view rendered by openGL.  Must be locking before any R/W operations
bool renderingStopped = false;

vector< Mat> planePaths;
vector< Aircraft*> myplanes;

int planeSelection = 0;
int widthOfAirspace = 4000;
Camera camera;

vector<Mat> PlanePathMatrices;
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

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // uncomment to enable Wireframe mode

	Shader skyboxShader(".\\Shaders\\Skybox\\skybox.vs", ".\\Shaders\\Skybox\\skybox.fs");
	Skybox * skybox = new Skybox(skyboxShader);

	//create some cubes!
	//create a CubeDrawer
	Shader cubeShader(".\\Shaders\\Cube\\cube.vs", ".\\Shaders\\Cube\\cube.fs");
	Texture woodBoxTexture(".\\asset\\container.jpg");
	Texture acmeTexture(".\\asset\\acme.jpg");
	CubeDrawer * cubeDrawer = new CubeDrawer(woodBoxTexture, acmeTexture, cubeShader);

	// PathHelper for preloaded paths
	PathHelper * pathHelper = new PathHelper();

	// create a plane that follows a path
	// create a plane that follows a path
	Shader planeShader(".\\Shaders\\Aircraft\\aircraft.vs", ".\\Shaders\\Aircraft\\aircraft.fs");

	
	//Create Planes Before Drawing any new windows
	PlaneGenerator planeGenerator(RANDOM, widthOfAirspace);
    myplanes = planeGenerator.getPlanes();
	
	PlanePathMatrices = planeGenerator.getPlanePaths();
	namedWindow("Plane Paths", CV_WINDOW_AUTOSIZE);
	cv::moveWindow("Plane Paths", 400, 10);
	createTrackbar("Plane Select: ", "Plane Paths", &planeSelection, planeGenerator.getPlanePaths().size() - 1, on_trackbar);
	

	PlaneDrawer * planeDrawer = new PlaneDrawer(woodBoxTexture, planeShader);

	// create camera and path for camera (our plane)
	camera = Camera(width, height, glm::vec3(0.0f, 0.0f, 1000.0f));
	camera.SetPath(pathHelper->GetPreloadedPath(0));
	camera.ActivateAutonomousMode();
	//camera.GetPath()->SetAvoidanceWaypoint(new Waypoint(glm::vec3(0.0f, 100.0f, -2000.0f)));

	// we have to create openCV windows in this thread!
	namedWindow("Blob Detection", CV_WINDOW_NORMAL);
	cv::moveWindow("Blob Detection", 600, 10);

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
		planeDrawer->Draw(view, projection, camera.GetPosition(), timeValue, myplanes);

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
	Mat frame, canny_output;

	// Setup SimpleBlobDetector parameters.
	SimpleBlobDetector::Params params;

	// Change thresholds
	params.minThreshold = 120;
	params.maxThreshold = 640;

	//Filter by Color
	params.filterByColor = true;
	params.blobColor = 255;

	// Filter by Area.
	params.filterByArea = true;
	params.minArea = 10;
	params.maxArea = 28000;

	// Filter by Circularity
	params.filterByCircularity = false;
	params.minCircularity = 0.1;

	// Filter by Convexity
	params.filterByConvexity = false;
	params.minConvexity = 0.87;

	// Filter by Inertia
	params.filterByInertia = false;
	params.minInertiaRatio = 0.01;

	// Storage for blobs
	vector<KeyPoint> keypoints;
	BlobTracker tracker = BlobTracker(30);

	while (!renderingStopped) {

		semaphore.lock();
		frame = img;
		semaphore.unlock();
		if (frame.rows > 0 && frame.cols > 0) {
			vector<vector<Point> > contours;
			vector<Vec4i> hierarchy;

			cvtColor(frame, frame, CV_RGB2GRAY);
			// Detect edges using canny
			Canny(frame, canny_output, 260, 380, 3);
			//imshow("After Canny", canny_output);
			dilate(canny_output, canny_output, cv::Mat(), cv::Point(-1, -1), 10);

			// Find contours
			findContours(canny_output, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

			// Draw contours
			Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
			for (int i = 0; i< contours.size(); i++) {
				Scalar color = Scalar(255, 255, 255); // THIS DOES RANDOM COLORS - rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
				drawContours(drawing, contours, i, color, -10, 8, hierarchy, 0, Point());
			}

			//Build blob detector
			SimpleBlobDetector detector(params);

			// Detect blobs
			detector.detect(drawing, keypoints);

			// Draw detected blobs as red circles.
			// DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures
			// the size of the circle corresponds to the size of blob
			Mat im_with_keypoints;
			drawKeypoints(drawing, keypoints, im_with_keypoints, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);


			/* THIS SECTION WILL START LOOKING AT HOW WE CAN KEEP TRACK OF WHAT BLOBS HAVE BEEN DETECTED*/

			cv::Size s = im_with_keypoints.size();
			int rows = s.height;
			int cols = s.width;
			Point center = Point(cols / 2, rows / 2); int centerRows = rows / 2; int centerCols = cols / 2;

			tracker.AddFrame(keypoints);

			vector<BlobInfo> info = tracker.GetBlobInfo(center);

			//This loop draws lines and circles on key elements of interest
			for (int i = 0; i < info.size(); i++) {
				if (info[i].foundPct >= 0.6) {
					double weight = info[i].GetCollisionValue();
					circle(im_with_keypoints, Point(info[i].currentPositionX, info[i].currentPositionY), weight / 250 * 50, Scalar(0, 0, weight), 4, 8);
					MyLine(im_with_keypoints,
						Point(info[i].currentPositionX, info[i].currentPositionY),
						Point(info[i].currentPositionX - info[i].deltaX, info[i].currentPositionY - info[i].deltaY),
						200, 200, 200);
					//cout << "Weight: " << weight << endl;
					//cout << "Active waypoint: " << active.x << " " << active.y << " "<< active.z <<  endl;// " location: " << current.x << " " << current.z << endl;
					//cout << "Current Location: " << -current.x << " " << -current.y << " " << -current.z <<  endl;// " location: " << current.x << " " << current.z << endl;
					if (weight > 240) {
						glm::vec3 active = camera.GetPath()->GetNextPathWaypoint()->GetPosition();
						glm::vec3 current = camera.GetPosition();
						Waypoint *newWay = new Waypoint(glm::vec3((-active.x + current.x) / 2.0, (current.y - 100.0), (active.z + current.z) / 2.0));
						cout << "Current pos: " << current.x << ", " << current.y << ", " << current.z << ", " << endl;
						cout << "Active WP pos: " << active.x << ", " << active.y << ", " << active.z << ", " << endl;
						cout << "Setting new WP: " << newWay->GetPosition().x << ", " << newWay->GetPosition().y << ", " << newWay->GetPosition().z << ", " << endl;
						double dist2 = point2pointDistance2(newWay->GetPosition().x, newWay->GetPosition().z, current.x, current.z);
						cout << dist2 << endl;
						if (dist2 < pow(400, 2)) {
							glm::vec3 direction = glm::normalize(newWay->GetPosition() + current);
							newWay = new Waypoint(glm::vec3(-300, -100, -800));
						}

						camera.GetPath()->SetAvoidanceWaypoint(newWay);
					}


				}
			}

			double step = rows / 5;
			for (int i = 1; i < 6; i++) {
				circle(im_with_keypoints, center, i*step, Scalar(0.0f, 255 * (1 - i / 6.0), 0.0f), 5);
			}
			imshow("Blob Detection", im_with_keypoints);
		}

	}
	return 0;
}


void on_trackbar(int, void*) {
	Mat local = PlanePathMatrices.at(planeSelection).clone();
	if (planeSelection == myplanes.size()) {
		for (int i = 0; i < myplanes.size(); i++) {
			stringstream value; value << i;
			//circle(local, Point((myplanes.at(i)->position.x + widthOfAirspace/2) / (widthOfAirspace / 500), (myplanes.at(i)->position.z + widthOfAirspace / 2) / (widthOfAirspace / 500)), 10, Scalar(0, 0, 0), 3);
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
		circle(local, Point((camera.GetPath()->GetActiveWaypoint()->GetPosition().x + widthOfAirspace / 2) / (widthOfAirspace / 500), (camera.GetPath()->GetActiveWaypoint()->GetPosition().z + widthOfAirspace / 2) / (widthOfAirspace / 500)), 10, Scalar(180, 105, 255), -1);
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



float point2pointDistance2(int pt1x, int pt1y, int pt2x, int pt2y) {
	return  pow((pt1x - pt2x), 2) + pow((pt1y - pt2y), 2);
}

float point2pointAngle(int pt1x, int pt1y, int pt2x, int pt2y) {
	if (pt1x - pt2x != 0)
		return  atan((pt1y - pt2y) / (pt1x - pt2x));
	else
		return atan((pt1y - pt2y) / (0.000001));
}

void MyLine(Mat img, Point start, Point end, int red, int green, int blue)
{
	int thickness = 3;
	int lineType = 8;
	line(img,
		Point((start.x), (start.y )),
		Point((end.x ) , (end.y)),
		Scalar(red, green, blue),
		thickness,
		lineType);
}