#pragma once

/*
This class keeps track of the constants associated with the different types of aircrafts we have available. 
According to the size of each aircraft, we want to be sure the collision radii are correctly calibrated.
*/

struct AircraftTableData {
	double wingspan;
	double focalLength;
	double ignoreSize; // anything below this size will be ignored
	double pointMassSize; // how big is the blob when the object is just a point in our FOV
};

class AircraftTable {
public:
	// best case is a huge aircraft like a 747 or an airbus a380
	static AircraftTableData getBestCase() {
		AircraftTableData boeing747;
		boeing747.wingspan = 225;
		boeing747.focalLength = 465; 
		boeing747.pointMassSize = 8;
		boeing747.ignoreSize = 50;
		return boeing747;
	}
	//typical case is a mid-sized plane
	static AircraftTableData getTypicalCase() {
		AircraftTableData fighter;
		fighter.wingspan = 130;
		fighter.focalLength = 300;
		fighter.pointMassSize = 20;
		fighter.ignoreSize = 25;
		return fighter;
	}
	//worst case is a small passenger plane like a cessna 172
	static AircraftTableData getWorstCase() {
		AircraftTableData cessna;
		cessna.wingspan = 36;
		cessna.focalLength = 500;
		cessna.pointMassSize = 16;
		cessna.ignoreSize = 10;
		return cessna;
	}

	//returns wingspan of the models
	static double GetPlaneModelWingspan() {
		return 110;
	}
	static double GetFighterModelWingspan() {
		return 400;
	}
	static double GetCessnaModelWingspan() {
		return 950; //this dude is huge
	}

	static double calculateApproximateDistance(double wingspan, double focalLength, double observedSizeInPx) {
		return wingspan*focalLength / observedSizeInPx;
	}
};