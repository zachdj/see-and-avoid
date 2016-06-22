#pragma once

#include "AvoidanceAlgorithm.h"

class AtcAvoidance : AvoidanceAlgorithm {
public:
	void reactToBlob(BlobInfo info, Camera & camera);
};