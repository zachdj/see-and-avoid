#include "AvoidanceWithDistance.h"

void AvoidanceWithDistance::reactToBlob(BlobInfo info, Camera & camera) {
	//if(camera.IsAutonomousNavigationActive()){
		AircraftTableData tableData = AircraftTable::getBestCase();
		double blobSize = info.currentSize - tableData.pointMassSize;
		/*double distance = sqrt(Utility::point2pointDistance2(
			camera.GetPosition().x, camera.GetPosition().z, myplanes[i]->position.x, myplanes[i]->position.z));
		focalLengthSum += blobSize * distance / tableData.wingspan;
		focalLengthMeasurements++;
		double avgFocalLength = focalLengthSum / focalLengthMeasurements;*/
		double calculatedDist = AircraftTable::calculateApproximateDistance(tableData.wingspan, tableData.focalLength, blobSize);
		// new stuff below

		double oldDist = AircraftTable::calculateApproximateDistance(tableData.wingspan, tableData.focalLength, blobSize - info.deltaSize);
		double deltaZ = calculatedDist - oldDist;

		double horizontalCrossSectionLength = calculatedDist * 2 * sin(20.0 * 3.14159 / 180.0);
		double horizontalPct = (info.currentPositionX - 960.0 / 2.0) / 960.0;
		double currentX = horizontalPct * horizontalCrossSectionLength;
		double oldHorizontalCrossSectionLength = oldDist * 2 * sin(20.0 * 3.14159 / 180.0);
		double oldHorizontalPct = (info.currentPositionX + info.deltaX - 960.0 / 2.0) / 960.0;
		double oldX = oldHorizontalPct * oldHorizontalCrossSectionLength;
		double deltaX = currentX - oldX;

		double verticalCrossSectionLength = calculatedDist * 2 * sin(20.0 * 3.14159 / 180.0);
		double verticalPct = (info.currentPositionY - 540.0 / 2.0) / 540.0;
		double currentY = verticalPct * verticalCrossSectionLength;
		double oldVerticalCrossSectionLength = oldDist * 2 * sin(20.0 * 3.14159 / 180.0);
		double oldVerticalPct = (info.currentPositionY + info.deltaY - 540.0 / 2.0) / 540.0;
		double oldY = oldVerticalPct * oldVerticalCrossSectionLength;
		double deltaY = currentY - oldY;

		double deltaTime = info.deltaTime;

		//get obstacle position and velocity relative to our current direction
		glm::vec3 obstacleVelocity = glm::vec3(deltaX / deltaTime, deltaY / deltaTime, deltaZ / deltaTime);
		glm::vec3 obstaclePosition = glm::vec3(currentX, currentY, calculatedDist);

		double cameraVelocity = camera.GetSpeed();
		double minDistTime = -(2 * obstaclePosition.x*obstacleVelocity.x
			+ 2 * obstaclePosition.y*obstacleVelocity.y
			+ 2 * obstaclePosition.z*obstacleVelocity.z - 2 * obstaclePosition.z*cameraVelocity)
			/
			(2 * obstacleVelocity.x + 2 * obstacleVelocity.y
				+ 2 * pow(obstacleVelocity.z - cameraVelocity, 2));

		double minDistSqrd = pow(obstaclePosition.x + minDistTime*obstacleVelocity.x, 2)
			+ pow(obstaclePosition.y + minDistTime*obstacleVelocity.y, 2)
			+ pow(obstaclePosition.z + minDistTime*(obstacleVelocity.z - cameraVelocity), 2);

		cout << "Obstacle Position: <" << obstaclePosition.x << ", " << obstaclePosition.y << ", " << obstaclePosition.z << ">" << endl;
		cout << "Obstacle Velocity: <" << obstacleVelocity.x << ", " << obstacleVelocity.y << ", " << obstacleVelocity.z << ">" << endl;
		cout << "DeltaTime: " << info.deltaTime << endl;
		cout << "Min Dist Time: " << minDistTime << endl;
		cout << "Min Dist: " << sqrt(minDistSqrd) << endl;
	//}


	//double weight = info.GetCollisionValue();
	//if (weight > 240 && camera.IsAutonomousNavigationActive()) {
	//	glm::vec3 active = camera.GetPath()->GetNextPathWaypoint()->GetPosition();
	//	glm::vec3 current = camera.GetPosition();

	//	glm::vec3 vectorToActive = active - current;

	//	glm::vec3 vectorToActiveDown = vectorToActive;
	//	vectorToActiveDown.y -= 200.0f;
	//	glm::vec3 normal = glm::normalize(glm::cross(vectorToActive, vectorToActiveDown));
	//	Waypoint * newWay = new Waypoint(current + vectorToActiveDown*0.5f + normal*10.0f);

	//	camera.GetPath()->SetAvoidanceWaypoint(newWay);
	//}

}
