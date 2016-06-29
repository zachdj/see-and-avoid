
/*
Tracked blob is the class used to obtain information about the currently tracked blob. 
*/

#pragma once

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <chrono>		// for ms-granularity time
#include <iostream>

using namespace std::chrono;

class TrackedBlob
{
public:
	TrackedBlob();
	TrackedBlob(double posX, double poxY, double size);
	TrackedBlob(unsigned int id, double posX, double poxY, double size);
	unsigned int GetId();
	~TrackedBlob();

	double posX;
	double posY;
	double size;
private:
	unsigned int id;
	
};

