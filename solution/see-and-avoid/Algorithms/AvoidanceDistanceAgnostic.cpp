#pragma once

#include "AvoidanceDistanceAgnostic.h"

void AvoidanceDistanceAgnostic::reactToBlob(BlobInfo info, Camera & camera) {
	double weight = info.GetCollisionValue();
	int deltaSize = info.deltaSize;
	int currentSize = info.currentSize;
	int deltaX = info.deltaX;
	if ((weight > 240 && camera.IsAutonomousNavigationActive() && currentSize > 75 && deltaSize > -5)|| deltaSize > 100 || currentSize > 250) {
		glm::vec3 active = camera.GetPath()->GetNextPathWaypoint()->GetPosition();
		glm::vec3 current = camera.GetPosition();

		glm::vec3 vectorToActive = active - current;

		glm::vec3 vectorToActiveDown = vectorToActive;
		vectorToActiveDown.y -= 200.0f;
		glm::vec3 normal = glm::normalize(glm::cross(vectorToActive, vectorToActiveDown));

		Waypoint * newWay;
		if(deltaX < 0) // move the opposite way that the plane is currently moving
			newWay = new Waypoint(current + vectorToActiveDown*0.5f  - normal*300.0f);
		else 
			newWay = new Waypoint(current + vectorToActiveDown*0.5f  + normal*300.0f);

		camera.GetPath()->SetAvoidanceWaypoint(newWay);
	}
}