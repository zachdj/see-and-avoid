#include "PathHelper.h"

PathHelper::PathHelper()
{
	this->scale = 1.0;
}
PathHelper::PathHelper(GLfloat scale)
{
	this->scale = scale;
}

PathHelper::~PathHelper() {};

Path * PathHelper::GetLinearPath() {
	vector<Waypoint *> waypoints;
	// Path 1 - Straight line with circly turnabouts for our camera
	waypoints.push_back(new Waypoint(this->scale*glm::vec3(0.0f, 0.0f, -500.0f)));
	waypoints.push_back(new Waypoint(this->scale*glm::vec3(400.0f, 0.0f, -900.0f)));
	waypoints.push_back(new Waypoint(this->scale*glm::vec3(300.0f, 0.0f, -1400.0f)));
	waypoints.push_back(new Waypoint(this->scale*glm::vec3(0.0f, 0.0f, -1000.0f)));
	waypoints.push_back(new Waypoint(this->scale*glm::vec3(0.0f, 0.0f, -500.0f)));
	waypoints.push_back(new Waypoint(this->scale*glm::vec3(0.0f, 0.0f, 500.0f)));
	waypoints.push_back(new Waypoint(this->scale*glm::vec3(-400.0f, 0.0f, 900.0f)));
	waypoints.push_back(new Waypoint(this->scale*glm::vec3(-300.0f, 0.0f, 1400.0f)));
	waypoints.push_back(new Waypoint(this->scale*glm::vec3(0.0f, 0.0f, 1000.0f)));
	waypoints.push_back(new Waypoint(this->scale*glm::vec3(0.0f, 0.0f, 500.0f)));
	return new Path(waypoints, 40.0f);
}


Path * PathHelper::GetCircularPath() {
	vector<Waypoint *> waypoints;
	int numPts = 15;
	float angleStep = 360.0 / numPts;
	float radius = -1500.0;
	for (int i = 0; i < numPts; i++) {
		float angle = (angleStep * i)*3.14159 / 180;
		float x = cos(angle)*radius;
		float z = sin(angle)*radius;
		waypoints.push_back(new Waypoint(this->scale*glm::vec3(x, 0.0f, z)));
	}
	return new Path(waypoints, 40.0f);
}


Path * PathHelper::GetFigureEightPath() {
	vector<Waypoint *> waypoints;
	waypoints.push_back(new Waypoint(this->scale*glm::vec3(-1500.0f, 0.0f, -1500.0f)));
	waypoints.push_back(new Waypoint(this->scale*glm::vec3(1500.0f, 0.0f, 1500.0f)));
	waypoints.push_back(new Waypoint(this->scale*glm::vec3(1500.0f, 0.0f, -1500.0f)));
	waypoints.push_back(new Waypoint(this->scale*glm::vec3(-1500.0f, 0.0f, 1500.0f)));
	return new Path(waypoints, 40.0f);
}

Path* PathHelper::GetStraightLine() {
	vector<Waypoint *> waypoints;
	waypoints.push_back(new Waypoint(this->scale*glm::vec3(0.0f, 0.0f, -5000.0f)));
	return new Path(waypoints, 40.0f);
}
