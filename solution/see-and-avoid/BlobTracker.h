
/*
This class created contains the methods used to track a blob we have found using OpenCV. We want to track a certain blob and give it a 
unique ID so that we are aware of how they move. 
*/

#pragma once

#include "TrackedBlob.h"
#include "BlobInfo.h"

#include <vector>
#include <opencv2\opencv.hpp>

using namespace std;

class BlobTracker
{
public:
	BlobTracker();
	BlobTracker(unsigned int historyLength);
	~BlobTracker();

	void AddFrame(vector<cv::KeyPoint> keypoints, float TimeValue);
	vector<BlobInfo> GetBlobInfo(cv::Point center);

private:
	static double pointToPointDistance2(double x1, double y1, double x2, double y2);
	static double pointToPointDistance2(TrackedBlob blob, cv::KeyPoint keypoint);

	unsigned int historyLength;
	vector< vector<TrackedBlob> > history; // stores tracked blobs from previous frames
	
};

