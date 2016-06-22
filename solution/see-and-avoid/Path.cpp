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
	if (this->loopBreakWaypoint != NULL && this->loopBreakWaypoint->IsActive()) {
		return this->loopBreakWaypoint;
	} else if (this->avoidanceWaypoint != NULL && this->avoidanceWaypoint->IsActive()) {
		return this->avoidanceWaypoint;
	} else {
		for (int i = 0; i < this->waypoints.size(); i++) {
			if (this->waypoints[i]->IsActive()) {
				return this->waypoints[i];
			}
		}
	}
	// if no active waypoint was found, then set all waypoints to active (i.e. loop through course again)
	for (int i = 0; i < this->waypoints.size(); i++) {
		this->waypoints[i]->Activate();
	}
	if (this->waypoints.size() > 0) {
		return this->waypoints[0];
	}
	return nullptr;
}

Waypoint* Path::GetNextPathWaypoint()
{
	for (int i = 0; i < this->waypoints.size(); i++) {
		if (this->waypoints[i]->IsActive()) {
			return this->waypoints[i];
		}
	}
	
	// if no active waypoint was found, then set all waypoints to active (i.e. loop through course again)
	for (int i = 0; i < this->waypoints.size(); i++) {
		this->waypoints[i]->Activate();
	}
	if (this->waypoints.size() > 0) {
		return this->waypoints[0];
	}
	return nullptr;
}

void Path::SetAvoidanceWaypoint(Waypoint * waypoint)
{
	this->loopBreakWaypoint = nullptr;
	this->avoidanceWaypoint = waypoint;
}


void Path::SetLoopBreakWaypoint(Waypoint * waypoint)
{
	this->avoidanceWaypoint = nullptr;
	this->loopBreakWaypoint = waypoint;
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
