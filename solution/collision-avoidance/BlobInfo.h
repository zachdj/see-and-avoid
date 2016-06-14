#pragma once
class BlobInfo
{
public:
	BlobInfo();
	BlobInfo(unsigned int id, double deltaX, double deltaY, double deltaSize, float foundPct);
	unsigned int GetId();
	~BlobInfo();

	double deltaX;
	double deltaY;
	double currentPositionX;
	double currentPositionY;
	double deltaSize;
	double foundPct;
private:
	unsigned int id;
};

