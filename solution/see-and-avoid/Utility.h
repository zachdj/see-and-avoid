#pragma once

#include <math.h>

class Utility {
public:
	static float point2pointAngle(int pt1x, int pt1y, int pt2x, int pt2y) {
		if (pt1x - pt2x != 0)
			return  atan((pt1y - pt2y) / (pt1x - pt2x));
		else
			return atan((pt1y - pt2y) / (0.000001));
	};

	static float point2pointDistance2(int pt1x, int pt1y, int pt2x, int pt2y) {
		return  pow((pt1x - pt2x), 2) + pow((pt1y - pt2y), 2);
	};



};