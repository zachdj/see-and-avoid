#include "TrackedBlob.h"


TrackedBlob::TrackedBlob()
{
	//srand(time(NULL));
	this->id = rand();
	this->posX = 0;
	this->posY = 0;
	this->size = 0;
}

TrackedBlob::TrackedBlob(double posX, double posY, double size)
{
	//srand(time(NULL));
	this->id = rand();
	this->posX = posX;
	this->posY = posY;
	this->size = size;
}

TrackedBlob::TrackedBlob(unsigned int id, double posX, double posY, double size)
{
	this->id = id;
	this->posX = posX;
	this->posY = posY;
	this->size = size;
}


unsigned int TrackedBlob::GetId()
{
	return this->id;
}



TrackedBlob::~TrackedBlob()
{
}
