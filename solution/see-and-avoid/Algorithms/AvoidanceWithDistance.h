#pragma once

#include "AvoidanceAlgorithm.h"
#include "../AircraftTable.h"
#include "../Utility.h"

class AvoidanceWithDistance : AvoidanceAlgorithm
{
public:
	void reactToBlob(BlobInfo info, Camera & camera);
};

