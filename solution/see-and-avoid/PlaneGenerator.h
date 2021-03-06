
/*
Class is used for building planes to put in the virutal world. We can either build planes with a given set of waypoints,  or build a set number of
planes randomly in the environment. Each has a particular research point to focus on.
*/


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
	//PlaneGenerator(bool random, int widthOfAirpace);
	static vector< Aircraft*> getPlanes();
	static vector< Mat> getPlanePaths();
	//~PlaneGenerator();
	static void generateAirspacePlanes(int widthOfAirspace);
	static void generateRandomPlanes(int widthOfAirspace);
	static void generateApproachingPlanes(int widthOfAirspace);
	static void generateAirportPlanes(int width);

	//All scenarios for testing for paper
	static void generate1Plane(int width, AircraftScale scale, int angle);
	static void generate2Planes(int width, AircraftScale scale1, AircraftScale scale2, int anglePlane1, int anglePlane2);
	static void generate3Planes(int width, AircraftScale scale1, AircraftScale scale2, AircraftScale scale3, int anglePlane1, int anglePlane2, int anglePlane3);

	

private:
	//static vector< Aircraft*> myPlanes;
	//static vector< Mat> planePaths;
	//static vector<Waypoint *> waypoints;
	//static vector<Point> points;
	//static vector<vector<Point>> planePoints;
	//static int widthOfAirspace;
	static void MyLine(Mat img, Point start, Point end, int red, int green, int blue, int widthOfAirspace);
	static void DrawPathsOnMatrix(int widthOfAirspace);

};

