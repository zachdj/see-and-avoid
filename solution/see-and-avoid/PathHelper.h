//Utility class for creating paths
#pragma once

#include "Path.h"
class PathHelper
{
public:
	PathHelper();
	PathHelper(GLfloat scale);
	~PathHelper();

	Path* GetLinearPath();
	Path* GetCircularPath();
	Path* GetFigureEightPath();
	Path* GetStraightLine();

private:
	GLfloat scale;
};

