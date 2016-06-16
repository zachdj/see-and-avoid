#pragma once

#include "Path.h"
class PathHelper
{
public:
	PathHelper();
	~PathHelper();

	Path* GetPreloadedPath(int identifier);

private:
	vector<Path*> preloadedPaths;

};

