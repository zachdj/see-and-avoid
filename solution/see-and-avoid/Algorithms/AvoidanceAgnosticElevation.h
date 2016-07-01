#pragma once

/*
This class is associated with the avoidance algorithm that does not changeElevation
*/

#include "AvoidanceAlgorithm.h"

class AvoidanceAgnosticElevation : AvoidanceAlgorithm {
public:
	void reactToBlob(BlobInfo info, Camera & camera);
};