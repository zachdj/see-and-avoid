#include "ContourVisionProcessor.h"



ContourVisionProcessor::ContourVisionProcessor()
{
	this->tracker = BlobTracker(30);
}

vector<BlobInfo> ContourVisionProcessor::ProcessScene(Mat img, bool shouldRender) {
	if (img.rows > 0 && img.cols > 0) {
		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;
		Mat canvas;

		//convert to grayscale
		cvtColor(img, canvas, CV_RGB2GRAY);
		// Detect edges using canny
		Canny(canvas, canvas, 260, 380, 3);
		dilate(canvas, canvas, cv::Mat(), cv::Point(-1, -1), 5);
		//smooth (blur) image to remove noise and plane textures
		blur(canvas, canvas, Size(20, 20));

		// Find contours
		findContours(canvas, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

		// Draw contours
		canvas = Mat::zeros(canvas.size(), CV_8UC3);
		for (int i = 0; i< contours.size(); i++) {
			Scalar color = Scalar(255, 255, 255); // THIS DOES RANDOM COLORS - rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			drawContours(canvas, contours, i, color, -10, 8, hierarchy, 0, Point());
		}

		vector<vector<Point> > contours_poly(contours.size());
		vector<Rect> boundRect(contours.size());
		vector<Point2f>contourCenters(contours.size());
		vector<float>radius(contours.size());
		vector<KeyPoint> keypoints;

		for (size_t i = 0; i < contours.size(); i++)
		{
			approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
			boundRect[i] = boundingRect(Mat(contours_poly[i]));
			minEnclosingCircle(contours_poly[i], contourCenters[i], radius[i]);
		}
		canvas = Mat::zeros(canvas.size(), CV_8UC3);
		for (size_t i = 0; i< contours.size(); i++)
		{
			Scalar color = Scalar(255,255,255);
			drawContours(canvas, contours_poly, (int)i, color, 1, 8, vector<Vec4i>(), 0, Point());
			//rectangle(canvas, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0);
			circle(canvas, contourCenters[i], (int)radius[i], color, 2, 8, 0);
			keypoints.push_back(KeyPoint(contourCenters[i], radius[i]));
		}

		/* This section tracks detected blobs using a BlobTracker */

		cv::Size s = canvas.size();
		int rows = s.height;
		int cols = s.width;
		Point center = Point(cols / 2, rows / 2); int centerRows = rows / 2; int centerCols = cols / 2;

		tracker.AddFrame(keypoints, glfwGetTime());

		vector<BlobInfo> info = tracker.GetBlobInfo(center);
		//This loop draws lines and circles on key elements of interest
		for (int i = 0; i < info.size(); i++) {
			if (info[i].foundPct >= 0.6) {
				double weight = info[i].GetCollisionValue();
				circle(canvas, Point(info[i].currentPositionX, info[i].currentPositionY), weight / 250 * 50, Scalar(0, 0, weight), 4, 8);
				line(canvas,
					Point(info[i].currentPositionX, info[i].currentPositionY),
					Point(info[i].currentPositionX - info[i].deltaX, info[i].currentPositionY - info[i].deltaY),
					Scalar(200, 200, 200), 3, 8);
			}
		}

		if (shouldRender) {
			// this loop draws circles on the radar
			double step = rows / 5;
			for (int i = 1; i < 6; i++) {
				circle(canvas, center, i*step, Scalar(0.0f, 255 * (1 - i / 6.0), 0.0f), 5);
			}
			imshow("Blob Detection", canvas);
		}

		return info;
	}
	return vector<BlobInfo>(0);
}


ContourVisionProcessor::~ContourVisionProcessor()
{
}
