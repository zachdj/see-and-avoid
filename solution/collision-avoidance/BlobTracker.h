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

	void AddFrame(vector<cv::KeyPoint> keypoints);
	vector<BlobInfo> GetBlobInfo(cv::Point center);

private:
	static double pointToPointDistance2(double x1, double y1, double x2, double y2);
	static double pointToPointDistance2(TrackedBlob blob, cv::KeyPoint keypoint);

	unsigned int historyLength;
	vector< vector<TrackedBlob> > history; // stores tracked blobs from previous frames
	
};

