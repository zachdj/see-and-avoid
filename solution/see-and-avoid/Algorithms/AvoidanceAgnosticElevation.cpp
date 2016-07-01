#pragma once

#include "AvoidanceAgnosticElevation.h"

void AvoidanceAgnosticElevation::reactToBlob(BlobInfo info, Camera & camera) {
	double weight = info.GetCollisionValue();
	int deltaSize = info.deltaSize;
	int currentSize = info.currentSize;
	int deltaX = info.deltaX;
	if (abs(camera.GetPosition().z - camera.GetPath()->GetActiveWaypoint()->GetPosition().z) < 40);

	else if ((weight > 220 && camera.IsAutonomousNavigationActive() && currentSize > 90 && deltaSize > -5) || deltaSize > 80 || currentSize > 140) {
		glm::vec3 active = camera.GetPath()->GetNextPathWaypoint()->GetPosition();
		glm::vec3 current = camera.GetPosition();

		glm::vec3 vectorToActive = active - current;

		//For drop when blob isnt moving
		glm::vec3 vectorToActiveDown = vectorToActive;
		vectorToActiveDown.y -= 400.0f;
		glm::vec3 normal = glm::normalize(glm::cross(vectorToActive, vectorToActiveDown));

		//For when we just want to move
		glm::vec3 vectorToActiveDown2 = glm::vec3(0.0f, -100.0f, 0.0f);
		glm::vec3 normalSamePlane = glm::normalize(glm::cross(vectorToActive, vectorToActiveDown2));


		Waypoint * newWay;
		if (deltaX < -1) // move the opposite way that the plane is currently moving
			newWay = new Waypoint(current * 0.5f + normal *100.0f);
		else // if (deltaX > 30)
			newWay = new Waypoint(current * 0.5f - normal *100.0f);
		//else
		//	newWay = new Waypoint(current + vectorToActiveDown*0.5f - normal*20.0f);

		camera.GetPath()->SetAvoidanceWaypoint(newWay);
	}
}