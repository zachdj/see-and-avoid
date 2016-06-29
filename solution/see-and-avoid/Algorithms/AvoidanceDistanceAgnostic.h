#pragma once

/*
This class is associated with the avoidance algorithm additional to ATC style. We want to build an avoidance 
that works without distance and at all costs. 
*/

#include "AvoidanceAlgorithm.h"

class AvoidanceDistanceAgnostic : AvoidanceAlgorithm {
public:
	void reactToBlob(BlobInfo info, Camera & camera);
};