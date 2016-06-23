#pragma once

//OpenCV includes
#include <opencv2/highgui/highgui.hpp>
#include <opencv2\opencv.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdlib.h> 

#include "BlobTracker.h"

using namespace cv;
class VisionProcessor
{
public:
	VisionProcessor();
	~VisionProcessor();

	vector<BlobInfo> ProcessScene(Mat img, bool shouldRender=true);

private:
	SimpleBlobDetector::Params params;
	vector<KeyPoint> keypoints;
	BlobTracker tracker;
};

