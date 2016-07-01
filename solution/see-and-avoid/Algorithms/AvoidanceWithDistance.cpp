#include "AvoidanceWithDistance.h"

void AvoidanceWithDistance::reactToBlob(BlobInfo info, Camera & camera) {
	if(/*camera.IsAutonomousNavigationActive()*/ true){
		AircraftTableData tableData = AircraftTable::getBestCase();
		double blobSize = info.currentSize - tableData.pointMassSize;
		double calculatedDist = AircraftTable::calculateApproximateDistance(tableData.wingspan, tableData.focalLength, blobSize);
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

		//cout << "Obstacle Position: <" << obstaclePosition.x << ", " << obstaclePosition.y << ", " << obstaclePosition.z << ">" << endl;
		//cout << "Obstacle Velocity: <" << obstacleVelocity.x << ", " << obstacleVelocity.y << ", " << obstacleVelocity.z << ">" << endl;
		//cout << "DeltaTime: " << info.deltaTime << endl;
		//cout << "Min Dist Time: " << minDistTime << endl;
		//cout << "Projected Min Dist: " << sqrt(minDistSqrd) << endl;

		if (minDistSqrd < 19600 && calculatedDist < 1000) {
			cout << "AVOIDING!" << endl << endl;
			//glm::vec3 direction = camera.GetCurrentDirectionFlat();
			//glm::vec3 position = camera.GetPosition();



			//glm::vec3 newWayPos = position + direction*((GLfloat)calculatedDist*0.75f);
			//newWayPos.y = position.y - 300;
			//Waypoint * newWay = new Waypoint(newWayPos);
			//camera.GetPath()->SetAvoidanceWaypoint(newWay);
		}
		else {
			cout << "NOT AVOIDING!" << endl << endl;
		}


	}


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
