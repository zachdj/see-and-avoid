//interface for avoidance algorithms

#pragma once

#include "../Waypoint.h"
#include "../Camera.h"
#include "../BlobInfo.h"
class AvoidanceAlgorithm
{
public:
	/*AvoidanceAlgorithm();
	~AvoidanceAlgorithm();*/
	virtual void reactToBlob(BlobInfo info, Camera & camera) = 0;
};

