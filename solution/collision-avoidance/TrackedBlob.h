#pragma once

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

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

