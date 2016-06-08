#include "Path.h"

Path::Path()
{
	vector<Waypoint *> waypoints;
	this->waypoints = waypoints;
	this->avoidanceWaypoint = nullptr;
	this->waypointCompletionRadius = 20.0f;
}

Path::Path(vector<Waypoint*> waypoints, GLfloat completionRadius)
{
	this->waypoints = waypoints;
	this->avoidanceWaypoint = nullptr;
	this->waypointCompletionRadius = completionRadius;
}

// either returns the collision avoidance waypoint or the next active waypoint along the route
Waypoint* Path::GetActiveWaypoint()
{
	if (this->avoidanceWaypoint != NULL && this->avoidanceWaypoint->IsActive()) {
		return this->avoidanceWaypoint;
	} else {
		for (int i = 0; i < this->waypoints.size(); i++) {
			if (this->waypoints[i]->IsActive()) {
				return this->waypoints[i];
			}
		}
	}
	// if no active waypoint was found, then return null
	return nullptr;
}

void Path::SetAvoidanceWaypoint(Waypoint * waypoint)
{
	this->avoidanceWaypoint = waypoint;
}

void Path::CompleteWaypoint()
{
	Waypoint * active = this->GetActiveWaypoint();
	if (active != nullptr) {
		active->Complete();
	}
}

Path::~Path()
{
}
