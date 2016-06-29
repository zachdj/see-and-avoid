#pragma once

/*
This class is associated with the avoidance algorithm similar to air traffic controller (ATC) style.
ATC style essentially just says change elevation to avoid an oncoming aircraft.
*/

#include "AvoidanceAlgorithm.h"

class AtcAvoidance : AvoidanceAlgorithm {
public:
	void reactToBlob(BlobInfo info, Camera & camera);
};