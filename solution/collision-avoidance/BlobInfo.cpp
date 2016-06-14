#include "BlobInfo.h"

BlobInfo::BlobInfo()
{
	this->id = 0;
	this->deltaX = 0;
	this->deltaY = 0;
	this->deltaSize = 0;
	this->foundPct = 0;
}

BlobInfo::BlobInfo(unsigned int id, double deltaX, double deltaY, double deltaSize, float foundPct)
{
	this->id = id;
	this->deltaX = deltaX;
	this->deltaY = deltaY;
	this->deltaSize = deltaSize;
	this->foundPct = foundPct;
}

unsigned int BlobInfo::GetId()
{
	return this->id;
}

BlobInfo::~BlobInfo()
{
}
