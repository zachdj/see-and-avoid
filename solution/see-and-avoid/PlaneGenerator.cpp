#include "PlaneGenerator.h"
#include <vector>

using namespace std;

PlaneGenerator::PlaneGenerator(bool randomPick)
{
	this->myRandom = randomPick;
	cout << " We got " << myRandom << endl;
	if (myRandom) {
		generateRandomPlanes();
	}
	else {
		generatePlanes();

	}
}


PlaneGenerator::~PlaneGenerator()
{
}

/*Generate Planes for later use*/
void PlaneGenerator::generatePlanes() {

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
	myPlanes.push_back(plane);

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
	myPlanes.push_back(plane2);

	//plane 3
	waypoints.clear();
	waypoints.push_back(new Waypoint(glm::vec3(0.0f, 0.0f, 0.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(0.0f, 0.0f, -1000.0f)));
	Path planePath3 = Path(waypoints, 20.0f);
	Aircraft* plane3 = new Aircraft(glm::vec3(0.0f, 0.0f, -200.0f), planePath3, 40.0f, ".\\Models\\vought\\vought.obj", 0.01);
	plane3->SetSpeed(50.0f);
	plane3->SetOrientation(0.0f, 180.0f, 0.0f);
	myPlanes.push_back(plane3);

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
	myPlanes.push_back(plane4);

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
	myPlanes.push_back(plane5);
}

void PlaneGenerator::generateRandomPlanes() {
	cout << "Enter the number of random generated planes..." << endl;
	int numberOfPlanes;
	cin >> numberOfPlanes;

	int widthOfAirspace = 2000;
	int waypointSize = 4;

	//As many planes as you want
	for (int i = 0; i < numberOfPlanes; i++) {
		waypoints.clear();
		for (int j = 0; j < waypointSize; j++) {
			float x = (float)(rand() % widthOfAirspace - widthOfAirspace / 2);
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
		myPlanes.push_back(plane);
		planePoints.push_back(points);
		points.clear();
	}
	DrawPathsOnMatrix();
	/*
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
	*/
}


void PlaneGenerator::DrawPathsOnMatrix() {
	int numberOfPlanes = myPlanes.size();
	Mat completeDrawing(500, 500, CV_8UC3, Scalar(255, 255, 255));
	for (int i = 0; i < numberOfPlanes; i++) {
		Mat individual(500, 500, CV_8UC3, Scalar(255, 255, 255));
		int red = rand() % 255; int green = rand() % 255; int blue = rand() % 255;
		for (int j = 1; j <= planePoints.at(i).size(); j++) {
			if (j != planePoints.at(i).size()) {
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

}


void PlaneGenerator::MyLine(Mat img, Point start, Point end, int red, int green, int blue)
{
	int thickness = 3;
	int lineType = 8;
	line(img,
		Point((start.x + 1000) / 4, (start.y + 1000) / 4),
		Point((end.x + 1000) / 4, (end.y + 1000) / 4),
		Scalar(red, green, blue),
		thickness,
		lineType);
}

vector< Aircraft*> PlaneGenerator::getPlanes() {
	return myPlanes;
}

vector< Mat> PlaneGenerator::getPlanePaths() {
	return planePaths;
}