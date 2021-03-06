#define PI 3.14159265

#include "PlaneGenerator.h"
#include <vector>
#include <sstream>

using namespace std;

static vector< Aircraft*> myPlanes;
static vector< Mat> planePaths;
static vector<Waypoint *> waypoints;
static vector<Point> points;
static vector<vector<Point>> planePoints;


/*Generate Planes for later use*/

void PlaneGenerator::generate1Plane(int width, AircraftScale scale, int angle) {
	int widthOfAirspace = width;

	waypoints.push_back(new Waypoint(glm::vec3(-5 * 1000 * sin(angle *PI / 180), 0.0f, -1000 + 5 * 1000 * cos(angle *PI / 180))));
	Path planePath = Path(waypoints, 20.0f);
	Aircraft * plane;
	if (scale == AircraftScale::big)
		plane = new Aircraft(glm::vec3(1000 * sin(angle *PI / 180), 0.0f, -1000 - 1000 * cos(angle *PI / 180)), planePath, ".\\Models\\plane\\plane.obj", AircraftScale::big);
	else if ((scale == AircraftScale::med))
		plane = new Aircraft(glm::vec3(1000 * sin(angle *PI / 180), 0.0f, -1000 - 1000 * cos(angle *PI / 180)), planePath, ".\\Models\\fighter\\fighter.obj", AircraftScale::med);
	else
		plane = new Aircraft(glm::vec3(1000 * sin(angle *PI / 180), 0.0f, -1000 - 1000 * cos(angle *PI / 180)), planePath, ".\\Models\\vought\\vought.obj", AircraftScale::small);
	plane->SetSpeed(50.0f);
	plane->SetOrientation(0.0f, (GLfloat)angle, 0.0f);
	myPlanes.push_back(plane);
	for (int i = 0; i < waypoints.size(); i++)
		points.push_back(Point(waypoints.at(i)->GetPosition().x, waypoints.at(i)->GetPosition().z));
	planePoints.push_back(points);
	points.clear();

	DrawPathsOnMatrix(widthOfAirspace);
}

void PlaneGenerator::generate2Planes(int width, AircraftScale scale1, AircraftScale scale2, int anglePlane1, int anglePlane2) {
	int widthOfAirspace = width;

	waypoints.push_back(new Waypoint(glm::vec3(-5 * 1000 * sin(anglePlane1 *PI / 180), 0.0f, -1000 +5* 1000 * cos(anglePlane1 *PI / 180))));
	Path planePath = Path(waypoints, 20.0f);
	Aircraft * plane;
	if (scale1 == AircraftScale::big)
		plane = new Aircraft(glm::vec3(1000 * sin(anglePlane1 *PI / 180), 0.0f, -1000 - 1000 * cos(anglePlane1 *PI / 180)), planePath, ".\\Models\\plane\\plane.obj", AircraftScale::big);
	else if ((scale1 == AircraftScale::med))
		plane = new Aircraft(glm::vec3(1000 * sin(anglePlane1 *PI / 180), 0.0f, -1000 - 1000 * cos(anglePlane1 *PI / 180)), planePath, ".\\Models\\fighter\\fighter.obj", AircraftScale::med);
	else
		plane = new Aircraft(glm::vec3(1000 * sin(anglePlane1 *PI / 180), 0.0f, -1000 - 1000 * cos(anglePlane1 *PI / 180)), planePath, ".\\Models\\vought\\vought.obj", AircraftScale::small);
	plane->SetSpeed(50.0f);
	plane->SetOrientation(0.0f, (GLfloat)anglePlane1, 0.0f);
	myPlanes.push_back(plane);
	for (int i = 0; i < waypoints.size(); i++)
		points.push_back(Point(waypoints.at(i)->GetPosition().x, waypoints.at(i)->GetPosition().z));
	planePoints.push_back(points);
	points.clear();

	waypoints.clear();
	waypoints.push_back(new Waypoint(glm::vec3(-5*1000 * sin(anglePlane2 *PI / 180), 0.0f, -1000 +5* 1000 * cos(anglePlane2 *PI / 180))));
	Path planePath2 = Path(waypoints, 20.0f);
	Aircraft * plane2;
	glm::vec3 direction2 = glm::vec3(1000 * sin(anglePlane2 *PI / 180), 0.0f, -1000 - 1000 * cos(anglePlane2 *PI / 180));
	if (scale2 == AircraftScale::big)
		plane2 = new Aircraft(direction2, planePath2, ".\\Models\\plane\\plane.obj", AircraftScale::big);
	else if ((scale2 == AircraftScale::med))
		plane2 = new Aircraft(direction2, planePath2, ".\\Models\\fighter\\fighter.obj", AircraftScale::med);
	else
		plane2 = new Aircraft(direction2, planePath2, ".\\Models\\vought\\vought.obj", AircraftScale::small);
	plane2->SetSpeed(50.0f);
	plane2->SetOrientation(0.0f, (GLfloat)anglePlane2, 0.0f);
	myPlanes.push_back(plane2);
	for (int i = 0; i < waypoints.size(); i++)
		points.push_back(Point(waypoints.at(i)->GetPosition().x, waypoints.at(i)->GetPosition().z));
	planePoints.push_back(points);
	points.clear();

	DrawPathsOnMatrix(widthOfAirspace);
}

void PlaneGenerator::generate3Planes(int width, AircraftScale scale1, AircraftScale scale2, AircraftScale scale3, int anglePlane1, int anglePlane2, int anglePlane3) {
	int widthOfAirspace = width;

	waypoints.push_back(new Waypoint(glm::vec3(-5 * 1000 * sin(anglePlane1 *PI / 180), 0.0f, -1000 + 5 * 1000 * cos(anglePlane1 *PI / 180))));
	Path planePath = Path(waypoints, 20.0f);
	Aircraft * plane;
	if (scale1 == AircraftScale::big)
		plane = new Aircraft(glm::vec3(1000 * sin(anglePlane1 *PI / 180), 0.0f, -1000 - 1000 * cos(anglePlane1 *PI / 180)), planePath, ".\\Models\\plane\\plane.obj", AircraftScale::big);
	else if ((scale1 == AircraftScale::med))
		plane = new Aircraft(glm::vec3(1000 * sin(anglePlane1 *PI / 180), 0.0f, -1000 - 1000 * cos(anglePlane1 *PI / 180)), planePath, ".\\Models\\fighter\\fighter.obj", AircraftScale::med);
	else
		plane = new Aircraft(glm::vec3(1000 * sin(anglePlane1 *PI / 180), 0.0f, -1000 - 1000 * cos(anglePlane1 *PI / 180)), planePath, ".\\Models\\vought\\vought.obj", AircraftScale::small);
	plane->SetSpeed(50.0f);
	plane->SetOrientation(0.0f, (GLfloat)anglePlane1, 0.0f);
	myPlanes.push_back(plane);
	for (int i = 0; i < waypoints.size(); i++)
		points.push_back(Point(waypoints.at(i)->GetPosition().x, waypoints.at(i)->GetPosition().z));
	planePoints.push_back(points);
	points.clear();

	waypoints.clear();
	waypoints.push_back(new Waypoint(glm::vec3(-5 * 1000 * sin(anglePlane2 *PI / 180), 0.0f, -1000 + 5 * 1000 * cos(anglePlane2 *PI / 180))));
	Path planePath2 = Path(waypoints, 20.0f);
	Aircraft * plane2;
	glm::vec3 direction2 = glm::vec3(1000 * sin(anglePlane2 *PI / 180), 0.0f, -1000 - 1000 * cos(anglePlane2 *PI / 180));
	if (scale2 == AircraftScale::big)
		plane2 = new Aircraft(direction2, planePath2, ".\\Models\\plane\\plane.obj", AircraftScale::big);
	else if ((scale2 == AircraftScale::med))
		plane2 = new Aircraft(direction2, planePath2, ".\\Models\\fighter\\fighter.obj", AircraftScale::med);
	else
		plane2 = new Aircraft(direction2, planePath2, ".\\Models\\vought\\vought.obj", AircraftScale::small);
	plane2->SetSpeed(50.0f);
	plane2->SetOrientation(0.0f, (GLfloat)anglePlane2, 0.0f);
	myPlanes.push_back(plane2);
	for (int i = 0; i < waypoints.size(); i++)
		points.push_back(Point(waypoints.at(i)->GetPosition().x, waypoints.at(i)->GetPosition().z));
	planePoints.push_back(points);
	points.clear();

	waypoints.clear();
	waypoints.push_back(new Waypoint(glm::vec3(-5 * 1000 * sin(anglePlane3 *PI / 180), 0.0f, -1000 + 5 * 1000 * cos(anglePlane3 *PI / 180))));
	Path planePath3 = Path(waypoints, 20.0f);
	Aircraft * plane3;
	glm::vec3 direction3 = glm::vec3(1000 * sin(anglePlane3 *PI / 180), 0.0f, -1000 - 1000 * cos(anglePlane3 *PI / 180));
	if (scale3 == AircraftScale::big)
		plane3 = new Aircraft(direction3, planePath3, ".\\Models\\plane\\plane.obj", AircraftScale::big);
	else if ((scale3 == AircraftScale::med))
		plane3 = new Aircraft(direction3, planePath3, ".\\Models\\fighter\\fighter.obj", AircraftScale::med);
	else
		plane3 = new Aircraft(direction3, planePath3, ".\\Models\\vought\\vought.obj", AircraftScale::small);
	plane3->SetSpeed(50.0f);
	plane3->SetOrientation(0.0f, (GLfloat)anglePlane3, 0.0f);
	myPlanes.push_back(plane3);
	for (int i = 0; i < waypoints.size(); i++)
		points.push_back(Point(waypoints.at(i)->GetPosition().x, waypoints.at(i)->GetPosition().z));
	planePoints.push_back(points);
	points.clear();

	DrawPathsOnMatrix(widthOfAirspace);
}



void PlaneGenerator::generateAirspacePlanes(int width) {

	int widthOfAirspace = width;
	////plane 1
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
	Aircraft* plane = new Aircraft(glm::vec3(0.0f, 0.0f, -1500.0f), planePath, ".\\Models\\plane\\plane.obj", AircraftScale::big);
	plane->SetSpeed(50.0f);
	myPlanes.push_back(plane);
	for (int i = 0; i < waypoints.size(); i++) 
		points.push_back(Point(waypoints.at(i)->GetPosition().x , waypoints.at(i)->GetPosition().z));
	planePoints.push_back(points);
	points.clear();

	//plane 2
	waypoints.clear();
	waypoints.push_back(new Waypoint(glm::vec3(1000.0f, 0.0f, 0.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(250.0f, 0.0f, 1000.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(-250.0f, 0.0f, 1000.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(-1000.0f, 0.0f, -0.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(-250.0f, 0.0f, -1000.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(250.0f, 0.0f, -1000.0f)));
	Path planePath2 = Path(waypoints, 20.0f);
	Aircraft* plane2 = new Aircraft(glm::vec3(0.0f, 0.0f, -100.0f), planePath2, ".\\Models\\plane\\plane.obj", AircraftScale::big);
	plane2->SetSpeed(100.0f);
	myPlanes.push_back(plane2);
	for (int i = 0; i < waypoints.size(); i++)
		points.push_back(Point(waypoints.at(i)->GetPosition().x, waypoints.at(i)->GetPosition().z));
	planePoints.push_back(points);
	points.clear();


	//plane 3
	waypoints.clear();
	waypoints.push_back(new Waypoint(glm::vec3(1000.0f, 0.0f, -1000.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(-1000.0f, 0.0f, -1000.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(1000.0f, 0.0f, 1000.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(-1000.0f, 0.0f, 1000.0f)));
	Path planePath3 = Path(waypoints, 20.0f);
	Aircraft* plane3 = new Aircraft(glm::vec3(0.0f, 0.0f, -200.0f), planePath3, ".\\Models\\vought\\vought.obj", AircraftScale::small);
	plane3->SetSpeed(50.0f);
	plane3->SetOrientation(0.0f, 180.0f, 0.0f);
	myPlanes.push_back(plane3);
	for (int i = 0; i < waypoints.size(); i++)
		points.push_back(Point(waypoints.at(i)->GetPosition().x, waypoints.at(i)->GetPosition().z));
	planePoints.push_back(points);
	points.clear();


	//plane 4
	waypoints.clear();
	waypoints.push_back(new Waypoint(glm::vec3(0.0f, 0.0f, -600.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(300.0f, 0.0f, 600.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(-300.0f, 0.0f, 600.0f)));
	Path planePath4 = Path(waypoints, 20.0f);
	Aircraft* plane4 = new Aircraft(glm::vec3(0.0f, 0.0f, -400.0f), planePath4, ".\\Models\\fighter\\fighter.obj", AircraftScale::med);
	plane4->SetSpeed(80.0f);
	myPlanes.push_back(plane4);
	for (int i = 0; i < waypoints.size(); i++)
		points.push_back(Point(waypoints.at(i)->GetPosition().x, waypoints.at(i)->GetPosition().z));
	planePoints.push_back(points);
	points.clear();


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
	Aircraft* plane5 = new Aircraft(glm::vec3(0.0f, 0.0f, -1000.0f), planePath5, ".\\Models\\plane\\plane.obj", AircraftScale::big);
	plane5->SetSpeed(100.0f);
	myPlanes.push_back(plane5);
	for (int i = 0; i < waypoints.size(); i++)
		points.push_back(Point(waypoints.at(i)->GetPosition().x, waypoints.at(i)->GetPosition().z));
	planePoints.push_back(points);
	points.clear();

	//plane 6
	waypoints.clear();
	waypoints.push_back(new Waypoint(glm::vec3(-1500, 0.0f, -1500.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(1500, 0.0f, 1500.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(1500, 0.0f, -1500.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(-1500, 0.0f, 1500.0f)));
	Path planePath6 = Path(waypoints, 20.0f);
	Aircraft* plane6 = new Aircraft(glm::vec3(-1500.0f, 0.0f, -1500.0f), planePath6, ".\\Models\\plane\\plane.obj", AircraftScale::big);
	plane6->SetSpeed(100.0f);
	myPlanes.push_back(plane6);
	for (int i = 0; i < waypoints.size(); i++)
		points.push_back(Point(waypoints.at(i)->GetPosition().x, waypoints.at(i)->GetPosition().z));
	planePoints.push_back(points);
	points.clear();

	DrawPathsOnMatrix(widthOfAirspace);

}

void PlaneGenerator::generateAirportPlanes(int width) {

	int widthOfAirspace = width;
	float defaultWidth = 4000.0f;
	float scale = widthOfAirspace / defaultWidth;
	//plane 1
	waypoints.clear();
	waypoints.push_back(new Waypoint(scale*glm::vec3(-1500.0f, -500.0f, -1000.0f))); // touch down
	waypoints.push_back(new Waypoint(scale*glm::vec3(-1500.0f, -500.0f, 1000.0f))); // end runway
	waypoints.push_back(new Waypoint(scale*glm::vec3(-1500.0f, 0.0f, 1500.0f)));
	waypoints.push_back(new Waypoint(scale*glm::vec3(1500.0f, 0.0f, 1500.0f)));
	waypoints.push_back(new Waypoint(scale*glm::vec3(1500.0f, 0.0f, -1500.0f)));
	waypoints.push_back(new Waypoint(scale*glm::vec3(-1500.0f, 0.0f, -1500.0f)));
	Path planePath1 = Path(waypoints, 20.0f);
	Aircraft* plane1 = new Aircraft(scale*glm::vec3(-1500.0f, 0.0f, -1500.0f), planePath1, ".\\Models\\plane\\plane.obj", AircraftScale::big);
	plane1->SetSpeed(50.0f);
	plane1->SetOrientation(0, 0, 0);
	myPlanes.push_back(plane1);
	for (int i = 0; i < waypoints.size(); i++)
		points.push_back(Point(waypoints.at(i)->GetPosition().x, waypoints.at(i)->GetPosition().z));
	planePoints.push_back(points);
	points.clear();

	//plane 2
	waypoints.clear();
	waypoints.push_back(new Waypoint(scale*glm::vec3(-1500.0f, 0.0f, -1500.0f)));
	waypoints.push_back(new Waypoint(scale*glm::vec3(-1500.0f, -500.0f, -1000.0f))); // touch down
	waypoints.push_back(new Waypoint(scale*glm::vec3(-1500.0f, -500.0f, 1000.0f))); // end runway
	waypoints.push_back(new Waypoint(scale*glm::vec3(-1500.0f, 0.0f, 1500.0f)));
	waypoints.push_back(new Waypoint(scale*glm::vec3(1500.0f, 0.0f, 1500.0f)));
	waypoints.push_back(new Waypoint(scale*glm::vec3(1500.0f, 0.0f, -1500.0f)));
	Path planePath2 = Path(waypoints, 20.0f);
	Aircraft * plane2 = new Aircraft(scale*glm::vec3(1500.0f, 0.0f, -1500.0f), planePath2, ".\\Models\\plane\\plane.obj", AircraftScale::big);
	plane2->SetSpeed(50.0f);
	plane2->SetOrientation(0, 90, 0);
	myPlanes.push_back(plane2);
	for (int i = 0; i < waypoints.size(); i++)
		points.push_back(Point(waypoints.at(i)->GetPosition().x, waypoints.at(i)->GetPosition().z));
	planePoints.push_back(points);
	points.clear();

	//plane 3
	waypoints.clear();
	waypoints.push_back(new Waypoint(scale*glm::vec3(1500.0f, 0.0f, -1500.0f)));
	waypoints.push_back(new Waypoint(scale*glm::vec3(-1500.0f, 0.0f, -1500.0f)));
	waypoints.push_back(new Waypoint(scale*glm::vec3(-1500.0f, -500.0f, -1000.0f))); // touch down
	waypoints.push_back(new Waypoint(scale*glm::vec3(-1500.0f, -500.0f, 1000.0f))); // end runway
	waypoints.push_back(new Waypoint(scale*glm::vec3(-1500.0f, 0.0f, 1500.0f)));
	waypoints.push_back(new Waypoint(scale*glm::vec3(1500.0f, 0.0f, 1500.0f)));
	Path planePath3 = Path(waypoints, 20.0f);
	Aircraft * plane3 = new Aircraft(scale*glm::vec3(1500.0f, 0.0f, 1500.0f), planePath3, ".\\Models\\plane\\plane.obj", AircraftScale::big);
	plane3->SetSpeed(50.0f);
	plane3->SetOrientation(0, 180, 0);
	myPlanes.push_back(plane3);
	for (int i = 0; i < waypoints.size(); i++)
		points.push_back(Point(waypoints.at(i)->GetPosition().x, waypoints.at(i)->GetPosition().z));
	planePoints.push_back(points);
	points.clear();


	//plane 4
	waypoints.clear();
	waypoints.push_back(new Waypoint(scale*glm::vec3(1500.0f, 0.0f, 1500.0f)));
	waypoints.push_back(new Waypoint(scale*glm::vec3(1500.0f, 0.0f, -1500.0f)));
	waypoints.push_back(new Waypoint(scale*glm::vec3(-1500.0f, 0.0f, -1500.0f)));
	waypoints.push_back(new Waypoint(scale*glm::vec3(-1500.0f, -500.0f, -1000.0f))); // touch down
	waypoints.push_back(new Waypoint(scale*glm::vec3(-1500.0f, -500.0f, 1000.0f))); // end runway
	waypoints.push_back(new Waypoint(scale*glm::vec3(-1500.0f, 0.0f, 1500.0f)));
	Path planePath4 = Path(waypoints, 20.0f);
	Aircraft * plane4 = new Aircraft(scale*glm::vec3(-1500.0f, 0.0f, 1500.0f), planePath4, ".\\Models\\plane\\plane.obj", AircraftScale::big);
	plane4->SetSpeed(50.0f);
	plane4->SetOrientation(0, 270, 0);
	myPlanes.push_back(plane4);
	for (int i = 0; i < waypoints.size(); i++)
		points.push_back(Point(waypoints.at(i)->GetPosition().x, waypoints.at(i)->GetPosition().z));
	planePoints.push_back(points);
	points.clear();


	//plane 5
	waypoints.clear();
	waypoints.push_back(new Waypoint(scale*glm::vec3(500.0f, 0.0f, -500.0f)));
	waypoints.push_back(new Waypoint(scale*glm::vec3(-1500.0f, 0.0f, -1500.0f)));
	waypoints.push_back(new Waypoint(scale*glm::vec3(-1500.0f, -500.0f, -1000.0f))); // touch down
	waypoints.push_back(new Waypoint(scale*glm::vec3(-1500.0f, -500.0f, 1000.0f))); // end runway
	waypoints.push_back(new Waypoint(scale*glm::vec3(-1500.0f, 0.0f, 1500.0f)));
	waypoints.push_back(new Waypoint(scale*glm::vec3(700.0f, 0.0f, 700.0f)));
	Path planePath5 = Path(waypoints, 20.0f);
	Aircraft * plane5 = new Aircraft(scale*glm::vec3(700.0f, 0.0f, 700.0f), planePath5, ".\\Models\\plane\\plane.obj", AircraftScale::big);
	plane5->SetSpeed(50.0f);
	plane5->SetOrientation(0, 180, 0);
	myPlanes.push_back(plane5);
	for (int i = 0; i < waypoints.size(); i++)
		points.push_back(Point(waypoints.at(i)->GetPosition().x, waypoints.at(i)->GetPosition().z));
	planePoints.push_back(points);
	points.clear();

	//plane 5
	waypoints.clear();
	waypoints.push_back(new Waypoint(scale*glm::vec3(1100.0f, 0.0f, -1100.0f)));
	waypoints.push_back(new Waypoint(scale*glm::vec3(-1500.0f, 0.0f, -1500.0f)));
	waypoints.push_back(new Waypoint(scale*glm::vec3(-1500.0f, -500.0f, -1000.0f))); // touch down
	waypoints.push_back(new Waypoint(scale*glm::vec3(-1500.0f, -500.0f, 1000.0f))); // end runway
	waypoints.push_back(new Waypoint(scale*glm::vec3(-1500.0f, 0.0f, 1500.0f)));
	waypoints.push_back(new Waypoint(scale*glm::vec3(1100.0f, 0.0f, 1100.0f)));
	Path planePath6 = Path(waypoints, 20.0f);
	Aircraft * plane6 = new Aircraft(scale*glm::vec3(1100.0f, 0.0f, 1100.0f), planePath6, ".\\Models\\plane\\plane.obj", AircraftScale::big);
	plane6->SetSpeed(50.0f);
	plane6->SetOrientation(0, 180, 0);
	myPlanes.push_back(plane6);
	for (int i = 0; i < waypoints.size(); i++)
		points.push_back(Point(waypoints.at(i)->GetPosition().x, waypoints.at(i)->GetPosition().z));
	planePoints.push_back(points);
	points.clear();
	
	DrawPathsOnMatrix(widthOfAirspace);

}


void PlaneGenerator::generateRandomPlanes(int width) {
	cout << "Enter the number of random generated planes..." << endl;
	int numberOfPlanes;
	cin >> numberOfPlanes;

	int widthOfAirspace = width;
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
			plane = new Aircraft(glm::vec3((float)(rand() % widthOfAirspace - widthOfAirspace / 2), 0.0f, (float)(rand() % widthOfAirspace - widthOfAirspace / 2)), planePath, ".\\Models\\vought\\vought.obj", AircraftScale::small);
		else if (planeType == 1)
			plane = new Aircraft(glm::vec3((float)(rand() % widthOfAirspace - widthOfAirspace / 2), 0.0f, (float)(rand() % widthOfAirspace - widthOfAirspace / 2)), planePath, ".\\Models\\fighter\\fighter.obj", AircraftScale::med);
		else
			plane = new Aircraft(glm::vec3((float)(rand() % widthOfAirspace - widthOfAirspace / 2), 0.0f, (float)(rand() % widthOfAirspace - widthOfAirspace / 2)), planePath, ".\\Models\\plane\\plane.obj", AircraftScale::small);
		plane->SetSpeed(50.0f);
		myPlanes.push_back(plane);
		planePoints.push_back(points);
		points.clear();
	}
	DrawPathsOnMatrix(widthOfAirspace);

}

void PlaneGenerator::generateApproachingPlanes(int width) {
	cout << "Enter the number of approaching planes...";
	int numberOfPlanes;
	cin >> numberOfPlanes;

	int widthOfAirspace = width;
	int start = -2500;

	//As many planes as you want
	for (int i = 0; i < numberOfPlanes; i++) {
		waypoints.clear();
		float x = (float)(rand() % widthOfAirspace - widthOfAirspace / 2);
		float y = (float)(rand() % 200 - 100);
			waypoints.push_back(new Waypoint(glm::vec3(x, y, start)));
			points.push_back(Point(x, start));
			waypoints.push_back(new Waypoint(glm::vec3(x, y,start+13000)));
			points.push_back(Point(x, start + 13000));
		
		Path planePath = Path(waypoints, 20.0f);
		int planeType = rand() % 3;
		Aircraft* plane;
		if (planeType == 0)
			plane = new Aircraft(glm::vec3(x, y, start), planePath, ".\\Models\\vought\\vought.obj", AircraftScale::small);
		else if (planeType == 1)
			plane = new Aircraft(glm::vec3(x, y, start), planePath, ".\\Models\\fighter\\fighter.obj", AircraftScale::med);
		else
			plane = new Aircraft(glm::vec3(x, y, start), planePath, ".\\Models\\plane\\plane.obj", AircraftScale::big);
		plane->SetSpeed(50.0f);
		myPlanes.push_back(plane);
		planePoints.push_back(points);
		points.clear();
		start -= 100;
	}
	DrawPathsOnMatrix(widthOfAirspace);

}



//This function takes our planes and draws a top down view of each of their paths. It also creates a total airspace flight path for all airplanes. 
//There are several loops in this function because we are iterating through all of the vectors we have available for our airplanes.
void PlaneGenerator::DrawPathsOnMatrix(int widthOfAirspace) {
	int numberOfPlanes = myPlanes.size();
	//Mat completeDrawing(500, 500, CV_8UC3, Scalar(255, 255, 255)); //Matrix for complete matrix with all paths
	Mat completeDrawing = imread("background.png");
	for (int i = 0; i < numberOfPlanes; i++) {
		Mat individual(500, 500, CV_8UC3, Scalar(255, 255, 255)); //Matrix for indvidual plane matrix with single paths
		stringstream width; width << widthOfAirspace;
		putText(individual, "Size of Airspace: " + width.str(), Point(0, 20), FONT_HERSHEY_DUPLEX, 0.5, Scalar(0, 0, 0)); //Print size of airspace on the Matrix
		int red = rand() % 255; int green = rand() % 255; int blue = rand() % 255; //random colors
		for (int j = 1; j <= planePoints.at(i).size(); j++) { //iteratre through all points to that individual plane
			if (j != planePoints.at(i).size()) { //draw lines accordingly on both matrices, while also puting point coordinates on the individual matrix
				MyLine(completeDrawing, planePoints.at(i).at(j), planePoints.at(i).at(j - 1), red, green, blue, widthOfAirspace);
				MyLine(individual, planePoints.at(i).at(j), planePoints.at(i).at(j - 1), red, green, blue, widthOfAirspace);
				stringstream x, y;
				x << planePoints.at(i).at(j-1).x;
				y << planePoints.at(i).at(j-1).y;
				putText(individual, "(" + x.str() + " , " + y.str() + ")", Point((planePoints.at(i).at(j - 1).x + widthOfAirspace / 2) / (widthOfAirspace / 500), (planePoints.at(i).at(j - 1).y + widthOfAirspace / 2) / (widthOfAirspace / 500)) , FONT_HERSHEY_PLAIN, 1, Scalar(red, green, blue));
			}
			else {
				MyLine(completeDrawing, planePoints.at(i).at(j - 1), planePoints.at(i).at(0), red, green, blue, widthOfAirspace);
				MyLine(individual, planePoints.at(i).at(j - 1), planePoints.at(i).at(0), red, green, blue, widthOfAirspace);
				stringstream x, y;
				x << planePoints.at(i).at(j - 1).x;
				y << planePoints.at(i).at(j - 1).y;
				putText(individual, "(" + x.str() + " , " + y.str() + ")", Point((planePoints.at(i).at(j - 1).x + widthOfAirspace / 2) / (widthOfAirspace / 500), (planePoints.at(i).at(j - 1).y + widthOfAirspace / 2) / (widthOfAirspace / 500)), FONT_HERSHEY_PLAIN, 1, Scalar(red, green, blue));
			}
		}
		planePaths.push_back(individual); //push individual matrix to the vector
	}
	planePaths.push_back(completeDrawing); //push complete matrix to the drawing

}


void PlaneGenerator::MyLine(Mat img, Point start, Point end, int red, int green, int blue, int widthOfAirspace)
{
	int thickness = 3;
	int lineType = 8;
	line(img,
		Point((start.x + widthOfAirspace / 2) / (widthOfAirspace / 500), (start.y + widthOfAirspace / 2) / (widthOfAirspace / 500)),
		Point((end.x + widthOfAirspace / 2) / (widthOfAirspace / 500), (end.y + widthOfAirspace / 2) / (widthOfAirspace / 500)),
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
