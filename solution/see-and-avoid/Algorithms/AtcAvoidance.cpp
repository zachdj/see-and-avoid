#pragma once

#include "AtcAvoidance.h"

void AtcAvoidance::reactToBlob(BlobInfo info, Camera & camera) {
	double weight = info.GetCollisionValue();
	if (weight > 240 && camera.IsAutonomousNavigationActive()) {
		glm::vec3 active = camera.GetPath()->GetNextPathWaypoint()->GetPosition();
		glm::vec3 current = camera.GetPosition();

		glm::vec3 vectorToActive = active - current;

		glm::vec3 vectorToActiveDown = vectorToActive;
		vectorToActiveDown.y -= 200.0f;
		glm::vec3 normal = glm::normalize(glm::cross(vectorToActive, vectorToActiveDown));
		Waypoint * newWay = new Waypoint(current + vectorToActiveDown*0.5f + normal*300.0f);

		camera.GetPath()->SetAvoidanceWaypoint(newWay);
	}
}