
/*
This class keeps track of the information associated with each blob. This information is used to track how an aircraft
may be moving, or how it may be approaching our own aircraft.
*/

#pragma once
class BlobInfo
{
public:
	BlobInfo();
	BlobInfo(unsigned int id, double deltaX, double deltaY, double deltaSize, float foundPct);
	unsigned int GetId();
	unsigned int GetCollisionValue();
	void SetCollisionValue(double val);
	~BlobInfo();

	double deltaX;
	double deltaY;
	double originalPositionX;
	double originalPositionY;
	double currentPositionX;
	double currentPositionY;
	double currentSize;
	double deltaSize;
	double foundPct;
private:
	unsigned int id;
	double collisionValue; // danger level
};

