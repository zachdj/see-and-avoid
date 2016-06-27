#pragma once
#include <vector>
#include "Aircraft.h"
#include "AircraftTable.h"

using namespace std;
#include <opencv2/highgui/highgui.hpp>
#include <opencv2\opencv.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdlib.h>     

using namespace cv;

class PlaneGenerator
{
public:
	PlaneGenerator(bool random, int widthOfAirpace);
	vector< Aircraft*> getPlanes();
	vector< Mat> getPlanePaths();
	~PlaneGenerator();

private:
	vector< Aircraft*> myPlanes;
	vector<Waypoint *> waypoints;
	vector<Point> points;
	vector<vector<Point>> planePoints;
	vector< Mat> planePaths;
	int widthOfAirspace;
	bool myRandom;

	void generatePlanes();
	void generateRandomPlanes();
	void generateApproachingPlanes();
	void MyLine(Mat img, Point start, Point end, int red, int green, int blue);
	void DrawPathsOnMatrix();

};

