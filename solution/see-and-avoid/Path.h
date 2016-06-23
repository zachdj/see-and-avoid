#pragma once

#include <glew.h>; // Include glew to get all the required OpenGL headers

#include <vector>
using namespace std;

#include "Waypoint.h"

// Path represents a navigational path for an aircraft.  It consists of a series of waypoints.  The aircraft will visit the waypoints in the order they are specified
class Path
{
public:
	Path();
	Path(vector<Waypoint *> waypoints, GLfloat completionRadius);
	Waypoint* GetActiveWaypoint();
	Waypoint* GetNextPathWaypoint(); // returns nextwaypoint on path, ignoring avoidance and loopBreak waypoints
	Waypoint* GetLoopBreakWaypoint(); // returns the current loop break waypoint
	void SetAvoidanceWaypoint(Waypoint * waypoint);
	void SetLoopBreakWaypoint(Waypoint * waypoint);
	void CompleteWaypoint();

	GLfloat waypointCompletionRadius;
	~Path();

private:
	vector<Waypoint *> waypoints;
	Waypoint * loopBreakWaypoint; // The loop break waypoint prevents situations where its impossible to reach the current WP because it's inside the turning radius.  It takes priority over every waypoint
	Waypoint * avoidanceWaypoint; // the avoidance waypoint is a waypoint we can set to avoid collisions.  It will always take top navigational priority over normal waypoints
};

