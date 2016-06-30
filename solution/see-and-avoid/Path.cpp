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

int Path::GetNextPathWaypointIndex()
{
	for (int i = 0; i < this->waypoints.size(); i++) {
		if (this->waypoints[i]->IsActive()) {
			return i;
		}
	}

	// if no active waypoint was found, then set all waypoints to active (i.e. loop through course again)
	for (int i = 0; i < this->waypoints.size(); i++) {
		this->waypoints[i]->Activate();
	}
	if (this->waypoints.size() > 0) {
		return 0;
	}
	return -1;
}

Waypoint * Path::GetLoopBreakWaypoint()
{
	return this->loopBreakWaypoint;
}

GLfloat Path::GetPredictorDeltaZ() {
	Waypoint * p1 = nullptr;
	Waypoint * p2 = nullptr;
	if (this->loopBreakWaypoint && this->loopBreakWaypoint->IsActive() 
		&& this->avoidanceWaypoint && this->avoidanceWaypoint->IsActive()) {
		p1 = this->loopBreakWaypoint;
		p2 = avoidanceWaypoint;
	} else if (this->loopBreakWaypoint && this->loopBreakWaypoint->IsActive()) {
		p1 = this->loopBreakWaypoint;
		p2 = this->GetNextPathWaypoint();
	} else if (this->avoidanceWaypoint && this->avoidanceWaypoint->IsActive()) {
		p1 = this->avoidanceWaypoint;
		p2 = this->GetNextPathWaypoint();
	} else {
		int index = this->GetNextPathWaypointIndex();
		p1 = this->waypoints[index];
		p2 = this->waypoints[(index + 1) % this->waypoints.size()];
	}
	
	if (p1 != nullptr && p2 != nullptr) {
		return p1->GetPosition().z - p2->GetPosition().z;
	}
	else return 0;
}

GLfloat Path::GetPredictorDeltaX() {
	Waypoint * p1 = nullptr;
	Waypoint * p2 = nullptr;
	if (this->loopBreakWaypoint && this->loopBreakWaypoint->IsActive()
		&& this->avoidanceWaypoint && this->avoidanceWaypoint->IsActive()) {
		p1 = this->loopBreakWaypoint;
		p2 = avoidanceWaypoint;
	}
	else if (this->loopBreakWaypoint && this->loopBreakWaypoint->IsActive()) {
		p1 = this->loopBreakWaypoint;
		p2 = this->GetNextPathWaypoint();
	}
	else if (this->avoidanceWaypoint && this->avoidanceWaypoint->IsActive()) {
		p1 = this->avoidanceWaypoint;
		p2 = this->GetNextPathWaypoint();
	}
	else {
		int index = this->GetNextPathWaypointIndex();
		p1 = this->waypoints[index];
		p2 = this->waypoints[(index + 1) % this->waypoints.size()];
	}

	if (p1 != nullptr && p2 != nullptr) {
		return p1->GetPosition().x - p2->GetPosition().x;
	}
	else return 0;
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
