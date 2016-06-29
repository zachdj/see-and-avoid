#include "BlobInfo.h"

BlobInfo::BlobInfo()
{
	this->id = 0;
	this->deltaX = 0;
	this->deltaY = 0;
	this->deltaSize = 0;
	this->deltaTime = 0;
	this->foundPct = 0;
	this->collisionValue = 0;
	this->currentPositionX = 0;
	this->currentPositionY = 0;
	this->currentSize = 0;
}

BlobInfo::BlobInfo(unsigned int id, double deltaX, double deltaY, double deltaSize, double deltaTime, float foundPct)
{
	this->id = id;
	this->deltaX = deltaX;
	this->deltaY = deltaY;
	this->deltaSize = deltaSize;
	this->deltaTime = deltaTime;
	this->foundPct = foundPct;
	this->collisionValue = 0;
	this->currentPositionX = 0;
	this->currentPositionY = 0;
	this->currentSize = 0;
}

unsigned int BlobInfo::GetId()
{
	return this->id;
}

unsigned int BlobInfo::GetCollisionValue()
{
	return this->collisionValue;

}

void BlobInfo::SetCollisionValue(double val)
{
	this->collisionValue = val;
}

BlobInfo::~BlobInfo()
{
}
