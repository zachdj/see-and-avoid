#include "PathHelper.h"

PathHelper::PathHelper()
{
	vector<Waypoint *> waypoints;
	/***************************** Preloaded Paths ********************************/
	
	// Path 1 - Straight line with circly turnabouts for our camera
	waypoints.push_back(new Waypoint(glm::vec3(0.0f, 0.0f, -500.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(400.0f, 0.0f, -900.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(300.0f, 0.0f, -1400.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(0.0f, 0.0f, -1000.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(0.0f, 0.0f, -500.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(0.0f, 0.0f, 500.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(-400.0f, 0.0f, 900.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(-300.0f, 0.0f, 1400.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(0.0f, 0.0f, 1000.0f)));
	waypoints.push_back(new Waypoint(glm::vec3(0.0f, 0.0f, 500.0f)));
	this->preloadedPaths.push_back( new Path(waypoints, 40.0f) );
	waypoints.clear();
	// End Path 1

	//Path 2 - Straight line with circle turnabouts for other planes
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
	this->preloadedPaths.push_back(new Path(waypoints, 40.0f));
	waypoints.clear();
	// End Path 2
}

PathHelper::~PathHelper() {};

Path* PathHelper::GetPreloadedPath(int identifier) {
	if (preloadedPaths.size() > 0) {
		return preloadedPaths[identifier % preloadedPaths.size()];
	}
	return nullptr;
}
