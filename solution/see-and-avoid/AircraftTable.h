#pragma once

/*
This class keeps track of the constants associated with the different types of aircrafts we have available. 
According to the size of each aircraft, we want to be sure the collision radii are correctly calibrated.
*/

struct AircraftTableData {
	double wingspan;
	double focalLength;
	double ignoreSize; // anything below this size should be considered too inaccurate to use
	double pointMassSize; // how big is the blob when the object is just a point in our FOV
};

class AircraftTable {
public:
	// best case is a huge aircraft like a 747 or an airbus a380
	static AircraftTableData getBoeing747() {
		AircraftTableData boeing747;
		boeing747.wingspan = 225;
		boeing747.focalLength = 465; 
		boeing747.pointMassSize = 8;
		boeing747.ignoreSize = 50;
		return boeing747;
	}
	static AircraftTableData getBoeing737() {
		AircraftTableData boeing737;
		boeing737.wingspan = 112;
		boeing737.focalLength = 520;
		boeing737.pointMassSize = 30;
		boeing737.ignoreSize = 25;
		return boeing737;
	}
	//typical case is a mid-sized plane
	static AircraftTableData getMidsizeJet() {
		AircraftTableData midsize;
		midsize.wingspan = 65;
		midsize.focalLength = 600; // TODO
		midsize.pointMassSize = 22.6; // TODO
		midsize.ignoreSize = 12; // TODO
		return midsize;
	}
	//worst case is a small passenger plane like a cessna 172
	static AircraftTableData getCessna172Case() {
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