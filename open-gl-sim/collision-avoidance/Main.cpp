//#include "opencv2/opencv.hpp"
#define _ITERATOR_DEBUG_LEVEL 0

#include<opencv2\highgui\highgui.hpp>
#include<iostream>
#include <opencv2\opencv.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
using namespace std;
using namespace cv;



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
	VideoCapture cap("PlaneSim.mp4");   //open the capture for video file
	int totalframe = cap.get(CV_CAP_PROP_FRAME_COUNT); // get total number of frames in video
	namedWindow("Canvas", CV_WINDOW_AUTOSIZE);
	cv::moveWindow("Canvas", 10, 20);
	while (waitKey(30) != 27 && totalframe>0)
		//wait 30 milliseconds and check for esc key
	{
		cap >> edges; //save captured image to frame variable
		//cvtColor(frame, edges, CV_BGR2GRAY);
		//GaussianBlur(edges, edges, Size(1, 1), 0.5, 0.5, BORDER_REFLECT);
		//blur(edges, edges, Size(7, 7));
		Canny(edges, edges, 180, 180, 3);
		cv::Size s = edges.size();
		int rows = s.height; cout << rows << endl;
		int cols = s.width; cout << "    " << cols << endl;
		//cout << Point(rows / 2 + 40, cols / 2 + 40) << endl;
		MyLine(edges, Point(cols/2 + 30, rows/2), Point(cols/2 - 30, rows/2 ));
		MyLine(edges, Point(cols / 2, rows / 2 + 30), Point(cols / 2, rows / 2 - 30));
		imshow("Canvas", edges); //show image on window named Camera
	
		totalframe--;
	}
}





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