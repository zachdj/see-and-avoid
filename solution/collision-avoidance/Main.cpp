//#include "opencv2/opencv.hpp"
#define _ITERATOR_DEBUG_LEVEL 0
#define PI 3.14159265

#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
//#include <opencv\cv.h>
#include <opencv2\opencv.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>

#include "BlobTracker.h"
#include "BlobInfo.h"


using namespace std;
using namespace cv;

bool firstTime = true;
vector<KeyPoint>::iterator blobIterator2;
vector<KeyPoint> keypointsOld;
vector<KeyPoint> keypointsOld2;
vector<KeyPoint> keypointsOld3;

void MyLine(Mat img, Point start, Point end, int color)
{
int thickness = 3;
int lineType = 8;
line(img,
start,
end,
CV_RGB(color, color, color),
thickness,
lineType);
}

float point2pointDistance2(int pt1x, int pt1y, int pt2x, int pt2y) {
	return  pow((pt1x - pt2x),2) + pow((pt1y - pt2y),2);
}

float point2pointAngle(int pt1x, int pt1y, int pt2x, int pt2y) {
	if (pt1x - pt2x != 0)
		return  atan((pt1y - pt2y) / (pt1x - pt2x));
	else
		return atan((pt1y - pt2y) / (0.000001));
}

void renderScene();
void processScene();

int main(void)
{
	Mat frame, canny_output;
	VideoCapture cap("PlaneSim3.mp4");   //open the capture for video file
	int totalframe = cap.get(CV_CAP_PROP_FRAME_COUNT); // get total number of frames in video
	namedWindow("Control", CV_WINDOW_NORMAL);
	cv::moveWindow("Control", 10, 10);
	namedWindow("Blob Detection", CV_WINDOW_NORMAL);
	cv::moveWindow("Blob Detection", 600, 10);

	// Setup SimpleBlobDetector parameters.
	SimpleBlobDetector::Params params;

	// Change thresholds
	params.minThreshold = 120;
	params.maxThreshold = 640;

	//Filter by Color
	params.filterByColor = true;
	params.blobColor = 255; 

	// Filter by Area.
	params.filterByArea = true;
	params.minArea = 10;
	params.maxArea = 28000;

	// Filter by Circularity
	params.filterByCircularity = false;
	params.minCircularity = 0.1;

	// Filter by Convexity
	params.filterByConvexity = false;
	params.minConvexity = 0.87;

	// Filter by Inertia
	params.filterByInertia = false;
	params.minInertiaRatio = 0.01;

	// Storage for blobs
	vector<KeyPoint> keypoints;

	//Reduce Frame count so we don't overuse our frames available
	totalframe--; totalframe--; totalframe--;

	BlobTracker tracker = BlobTracker(30);

	while (waitKey(10) != 27 && totalframe>0)
		//wait 10 milliseconds and check for esc key
	{
		cap >> frame; //save captured image to frame variable

        imshow("Control", frame);
		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;
		
		/// Detect edges using canny
		Canny(frame, canny_output, 260,380, 3);
		//imshow("After Canny", canny_output);
		dilate(canny_output, canny_output, cv::Mat(), cv::Point(-1, -1),10);

		/// Find contours
		findContours(canny_output, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
		
		/// Draw contours
		Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
		for (int i = 0; i< contours.size(); i++){
			Scalar color = Scalar(255, 255, 255); // THIS DOES RANDOM COLORS - rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			drawContours(drawing, contours, i, color, -10, 8, hierarchy, 0, Point());
		    }

		// Setup a rectangle to define your region of interest to crop
		cv::Rect myROI(30, 35, 1220, 580);

		// Crop the full image to that image contained by the rectangle myROI
		// Note that this doesn't copy the data
		drawing = drawing(myROI);

        //Build blob detector
		SimpleBlobDetector detector(params);

		// Detect blobs
		detector.detect(drawing, keypoints);

		// Draw detected blobs as red circles.
		// DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures
		// the size of the circle corresponds to the size of blob
		Mat im_with_keypoints;
		drawKeypoints(drawing, keypoints, im_with_keypoints, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);


		/* THIS SECTION WILL START LOOKING AT HOW WE CAN KEEP TRACK OF WHAT BLOBS HAVE BEEN DETECTED*/

		cv::Size s = im_with_keypoints.size();
		int rows = s.height; cout << rows;
		int cols = s.width; cout << "    " << cols << endl;
		Point center = Point(cols / 2, rows / 2); int centerRows = rows / 2; int centerCols = cols / 2;

		tracker.AddFrame(keypoints);
		
		vector<BlobInfo> info = tracker.GetBlobInfo(center);

		std::cout << "Current x: " << keypoints[keypoints.size() - 1].pt.x << std::endl;
		std::cout << "Current y: " << keypoints[keypoints.size() - 1].pt.y << std::endl;

		//This loop draws lines and circles on key elements of interest
		for (int i = 0; i < info.size(); i++) {
			if (info[i].foundPct >= 0.6) {
				circle(im_with_keypoints, Point(info[i].currentPositionX, info[i].currentPositionY), 50, Scalar(0, 255, 0), 8, 8);
				MyLine(im_with_keypoints, 
					Point(info[i].currentPositionX, info[i].currentPositionY), 
					Point(info[i].currentPositionX + info[i].deltaX, info[i].currentPositionY + info[i].deltaY),
					200);
			}
			
		}


		rectangle(im_with_keypoints, Point(center.x - cols / 10, center.y - rows / 10), Point(center.x + cols / 10, center.y + rows / 10), Scalar(0, 0, 250), 8, 8);
		rectangle(im_with_keypoints, Point(center.x - cols / 5, center.y - rows / 5), Point(center.x + cols / 5, center.y + rows / 5), Scalar(0, 250,0), 8, 8);
		rectangle(im_with_keypoints, Point(center.x - cols / 3, center.y - rows / 3), Point(center.x + cols / 3, center.y + rows / 3), Scalar(250, 0,0), 8, 8);


		//This loop will 
		for (int i = 0; i < info.size(); i++) {
			cout << "Weight: " << info[i].GetCollisionValue()<< endl;
		}

		imshow("Blob Detection", im_with_keypoints);
		//Subtract Frame and throw away 2 more. We do not need these many frames
		totalframe--; cap >> frame; totalframe--; cap >> frame; totalframe--;

	}
}




/*THIS IS ALL OF THE TEST WORK BEFORE I CLEANED IT UP*/

/*
using namespace std;
using namespace cv;

Mat fgMaskMOG; //fg mask generated by MOG method
Mat fgMaskMOG2; //fg mask fg mask generated by MOG2 method
Ptr<BackgroundSubtractor>  pMOG;
Ptr<BackgroundSubtractor>  pMOG2;
RNG rng(12345);


void MyLine(Mat img, Point start, Point end)
{
int thickness = 3;
int lineType = 8;
line(img,
start,
end,
CV_RGB(250,250,250),
thickness,
lineType);
}




int main(void)
{
Mat frame;
Mat edges;
VideoCapture cap("PlaneSim2.mp4");   //open the capture for video file
int totalframe = cap.get(CV_CAP_PROP_FRAME_COUNT); // get total number of frames in video
namedWindow("Control", CV_WINDOW_AUTOSIZE);
cv::moveWindow("Control", 10, 20);
//pMOG = new BackgroundSubtractorMOG(100,3,0.5); //MOG approach
//pMOG2 = new BackgroundSubtractorMOG2(0, 18, false); //MOG2 approach


// Setup SimpleBlobDetector parameters.
SimpleBlobDetector::Params params;

// Change thresholds
params.minThreshold = 120;
params.maxThreshold = 640;

//Filter by Color
params.filterByColor = true;
params.blobColor = 255;

// Filter by Area.
params.filterByArea = true;
params.minArea = 10;
params.maxArea = 28000;

// Filter by Circularity
params.filterByCircularity = false;
params.minCircularity = 0.1;

// Filter by Convexity
params.filterByConvexity = false;
params.minConvexity = 0.87;

// Filter by Inertia
params.filterByInertia = false;
params.minInertiaRatio = 0.01;


// Storage for blobs
vector<KeyPoint> keypoints;
//Ptr<BackgroundSubtractor>  pMOG = new BackgroundSubtractorMOG(); //MOG approach
//Ptr<BackgroundSubtractor>  pMOG2 = new BackgroundSubtractorMOG2(10,20,false); //MOG2 approach


totalframe--;
totalframe--;
totalframe--;


while (waitKey(10) != 27 && totalframe>0)
//wait 30 milliseconds and check for esc key
{
cap >> frame; //save captured image to frame variable
//cvtColor(frame, edges, CV_BGR2GRAY);
//GaussianBlur(frame, frame, Size(0, 0), 2.5, 2.5, BORDER_REFLECT);
//blur(edges, edges, Size(7, 7));
//Canny(edges, edges, 180, 180, 3);
//cv::Size s = edges.size();
//int rows = s.height; cout << rows << endl;
//int cols = s.width; cout << "    " << cols << endl;
//cout << Point(rows / 2 + 40, cols / 2 + 40) << endl;
//MyLine(edges, Point(cols/2 + 30, rows/2), Point(cols/2 - 30, rows/2 ));
//MyLine(edges, Point(cols / 2, rows / 2 + 30), Point(cols / 2, rows / 2 - 30));
//imshow("Canvas", frame); //show image on window named Camera

//THESE ARE FOR OUR BACKGROUND SUBTRACTION
//pMOG->operator()(frame, fgMaskMOG);
//pMOG2->operator()(frame, fgMaskMOG2);
//imshow("FG Mask MOG", fgMaskMOG);
//imshow("FG Mask MOG 2", fgMaskMOG2);


Mat src_gray = frame;
//cvtColor(frame, src_gray, CV_RGB2XYZ);
//blur(src_gray, src_gray, Size(3, 3));


imshow("Control", src_gray);
Mat canny_output;
vector<vector<Point> > contours;
vector<Vec4i> hierarchy;

/// Detect edges using canny
Canny(src_gray, canny_output, 260,380, 3);
//imshow("After Canny", canny_output);
dilate(canny_output, canny_output, cv::Mat(), cv::Point(-1, -1),10);

/// Find contours
findContours(canny_output, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));


/// Draw contours
Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
for (int i = 0; i< contours.size(); i++)
{
Scalar color = Scalar(255, 255, 255); //rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
drawContours(drawing, contours, i, color, -10, 8, hierarchy, 0, Point());
}


// Setup a rectangle to define your region of interest
cv::Rect myROI(10, 20, 1220, 640);

// Crop the full image to that image contained by the rectangle myROI
// Note that this doesn't copy the data
drawing = drawing(myROI);

/// Show in a window
//namedWindow("Contours", CV_WINDOW_AUTOSIZE);
//imshow("Contours", drawing);





/*

//inRange(detectBlue, Scalar(0,0,0), Scalar(255,255,255), detectBlue);
//imshow("Control", detectBlue);
//imshow("original", frame);

Mat changeContrast;
//cvtColor(frame, frame, CV_RGB2GRAY);
//frame.convertTo(changeContrast, -1, 1.9, 0); //increase the contrast (double)
//imshow("High Contrast", changeContrast);
//bitwise_or(detectBlue, changeContrast, changeContrast);

//imshow("bitwise", changeContrast);


//GaussianBlur(frame, frame, Size(0, 0), 2.2, 2.2, BORDER_REFLECT);
//imshow("blur", frame);
//Canny(changeContrast, edges,180, 180);

// Set up detector with params
Mat edges;
edges = frame;


SimpleBlobDetector detector(params);

edges = drawing;

//int top = (int)(0.05*edges.rows); int bottom = (int)(0.05*edges.rows);
//int left = (int)(0.05*edges.cols); int right = (int)(0.05*edges.cols);
//copyMakeBorder(edges, edges, top, bottom, left, right, BORDER_CONSTANT, Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255)));
// Detect blobs
detector.detect(edges, keypoints);

// Draw detected blobs as red circles.
// DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures
// the size of the circle corresponds to the size of blob

Mat im_with_keypoints;
drawKeypoints(edges, keypoints, im_with_keypoints, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

// Show blobs
imshow("blobs", im_with_keypoints);
//cv::moveWindow("blobs", 10, 20);

for (std::vector<cv::KeyPoint>::iterator blobIterator = keypoints.begin(); blobIterator != keypoints.end(); blobIterator++) {
	std::cout << "size of blob is: " << blobIterator->size << std::endl;
	std::cout << "point is at: " << blobIterator->pt.x << " " << blobIterator->pt.y << std::endl;
}




totalframe--;

cap >> frame;
totalframe--;
cap >> frame;
totalframe--;
	}
}




*/



/*
#include <opencv2\opencv.hpp>

using namespace cv;

int main(int, char**)
{
	VideoCapture cap(0); // open the default camera
	if (!cap.isOpened())  // check if we succeeded
		return -1;

	Mat edges;
	namedWindow("edges", 1);
	for (;;)
	{
		Mat frame;
		cap >> frame; // get a new frame from camera
		cvtColor(frame, edges, CV_BGR2GRAY);
		GaussianBlur(edges, edges, Size(7, 7), 1.5, 1.5);
		Canny(edges, edges, 0, 30, 3);
		imshow("edges", edges);
		if (waitKey(30) >= 0) break;
	}
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}

*/