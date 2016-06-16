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
	double deltaSize;
	double foundPct;
private:
	unsigned int id;
	double collisionValue;
};

