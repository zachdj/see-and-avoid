#pragma once

struct AircraftInfo {
	double wingspan;
	double focalLength;
};

class AircraftTable {
public:
	// best case is a huge aircraft like a 747 or an airbus a380
	static AircraftInfo getBestCase() {
		AircraftInfo boeing747;
		boeing747.wingspan = 225;
		boeing747.focalLength = 200; 
		return boeing747;
	}
	//typical case is a mid-sized plane
	static AircraftInfo getTypicalCase() {
		AircraftInfo typical;
		typical.wingspan = 130;
		typical.focalLength = 0; //TODO!!!
		return typical;
	}
	//worst case is a small passenger plane like a cessna 172
	static AircraftInfo getWorstCase() {
		AircraftInfo cessna;
		cessna.wingspan = 36;
		cessna.focalLength = 0; //TODO!!!
		return cessna;
	}

	//returns wingspan of the models
	static double GetPlaneModelWingspan() {
		return 110;
	}
	static double GetVoughtModelWingspan() {
		return 0; //TODO
	}
	static double GetCessnaModelWingspan() {
		return 0; //TODO
	}
};