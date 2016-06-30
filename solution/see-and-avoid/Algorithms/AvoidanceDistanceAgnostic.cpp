#pragma once

#include "AvoidanceDistanceAgnostic.h"

void AvoidanceDistanceAgnostic::reactToBlob(BlobInfo info, Camera & camera) {
	double weight = info.GetCollisionValue();
	int deltaSize = info.deltaSize;
	int currentSize = info.currentSize;
	int deltaX = info.deltaX;
	if (abs(camera.GetPosition().z - camera.GetPath()->GetActiveWaypoint()->GetPosition().z) < 40);

	else if ((weight > 220 && camera.IsAutonomousNavigationActive() && currentSize > 90 && deltaSize > -5)|| deltaSize > 80 || currentSize > 160) {
		glm::vec3 active = camera.GetPath()->GetNextPathWaypoint()->GetPosition();
		glm::vec3 current = camera.GetPosition();

		glm::vec3 vectorToActive = active - current;

		glm::vec3 vectorToActiveDown = vectorToActive;
		vectorToActiveDown.y -= 400.0f;
		glm::vec3 normal = glm::normalize(glm::cross(vectorToActive, vectorToActiveDown));

		Waypoint * newWay;
		if(deltaX < -50) // move the opposite way that the plane is currently moving
			newWay = new Waypoint(current + vectorToActiveDown*0.5f  + normal*150.0f);
		else if (deltaX > 50)
			newWay = new Waypoint(current + vectorToActiveDown*0.5f  );
		else 
			newWay = new Waypoint(current + vectorToActiveDown*0.5f- normal*20.0f);

		camera.GetPath()->SetAvoidanceWaypoint(newWay);
	}
}