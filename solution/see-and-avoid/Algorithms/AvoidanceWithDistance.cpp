#include "AvoidanceWithDistance.h"

void AvoidanceWithDistance::reactToBlob(BlobInfo info, Camera & camera) {
	double const SCREENW = 960.0;
	double const SCREENH = 540.0;

	if(/*camera.IsAutonomousNavigationActive()*/ true){
		AircraftTableData tableData = AircraftTable::getBestCase();
		double blobSize = info.currentSize - tableData.pointMassSize;
		double calculatedDist = AircraftTable::calculateApproximateDistance(tableData.wingspan, tableData.focalLength, blobSize);
		double oldDist = AircraftTable::calculateApproximateDistance(tableData.wingspan, tableData.focalLength, blobSize - info.deltaSize);
		double deltaZ = calculatedDist - oldDist;

		double horizontalCrossSectionLength = calculatedDist * 2 * sin(20.0 * 3.14159 / 180.0);
		double horizontalPct = (info.currentPositionX - SCREENW / 2.0) / SCREENW;
		double currentX = horizontalPct * horizontalCrossSectionLength;
		double oldHorizontalCrossSectionLength = oldDist * 2 * sin(20.0 * 3.14159 / 180.0);
		double oldHorizontalPct = (info.currentPositionX + info.deltaX - SCREENW / 2.0) / SCREENW;
		double oldX = oldHorizontalPct * oldHorizontalCrossSectionLength;
		double deltaX = currentX - oldX;

		double verticalCrossSectionLength = calculatedDist * 2 * sin(20.0 * 3.14159 / 180.0);
		double verticalPct = (info.currentPositionY - SCREENH / 2.0) / SCREENH;
		double currentY = verticalPct * verticalCrossSectionLength;
		double oldVerticalCrossSectionLength = oldDist * 2 * sin(20.0 * 3.14159 / 180.0);
		double oldVerticalPct = (info.currentPositionY + info.deltaY - SCREENH / 2.0) / SCREENH;
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
			glm::vec3 direction = camera.GetCurrentDirectionFlat();
			glm::vec3 position = camera.GetPosition();
			//glm::vec3 targetPos = camera.GetPath()->GetNextPathWaypoint()->GetPosition();
			glm::vec3 targetPos = camera.GetPath()->GetActiveWaypoint()->GetPosition();
			glm::vec3 targetDir = glm::normalize(targetPos - position);
			glm::vec3 targetDirDown = targetDir;
			targetDirDown.y -= 1;
			glm::vec3 normal = glm::cross(targetDir, targetDirDown);

			double distToTarget2 = pow(position.x - obstaclePosition.x, 2) + pow(position.y - obstaclePosition.y, 2) + pow(position.z - obstaclePosition.z, 2);
			double distToTarget = sqrt(distToTarget2);
			Waypoint * newWay;

			double theta = acos(glm::dot(direction, glm::vec3(0.0f, 0.0f, 1.0f)));
			if (theta > 2.5 || theta < 0.6) {
				if (info.currentPositionX <= SCREENW/2 && info.deltaX >= 0) {
					//if blob is on the left and moving right, turn hard left
					newWay = new Waypoint(position + 300.0f*direction + 175.0f*normal);
				}
				else if(info.currentPositionX <= SCREENW/2 && info.deltaX < 0) {
					//if blob is on the left and moving left, turn slight right
					newWay = new Waypoint(position + 250.0f*direction - 40.0f*normal);
					//uncomment below for harder turn
					//newWay = new Waypoint(position + 300.0f*direction - 175.0f*normal);
				}
				else if (info.currentPositionX > SCREENW / 2 && info.deltaX >= 0) {
					//if blob is on the right and moving right, turn slight left
					newWay = new Waypoint(position + 250.0f*direction + 40.0f*normal);
					//uncomment below for harder turn
					//newWay = new Waypoint(position + 300.0f*direction + 175.0f*normal);
				}
				else{ //if (info.currentPositionX > SCREENW / 2 && info.deltaX < 0) {
					//if blob is on the right and moving left, turn hard right
					newWay = new Waypoint(position + 300.0f*direction - 175.0f*normal);
				}				
				camera.GetPath()->SetAvoidanceWaypoint(newWay);
			}
		}
	}

}
