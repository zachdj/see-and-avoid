#include "VisionProcessor.h"



VisionProcessor::VisionProcessor()
{
	this->params = SimpleBlobDetector::Params();
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

	this->keypoints = vector<KeyPoint>();
	this->tracker = BlobTracker(30);
}

vector<BlobInfo> VisionProcessor::ProcessScene(Mat img, bool shouldRender) {
	if (img.rows > 0 && img.cols > 0) {
		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;
		Mat canvas;

		cvtColor(img, canvas, CV_RGB2GRAY);
		// Detect edges using canny
		Canny(canvas, canvas, 260, 380, 3);
		dilate(canvas, canvas, cv::Mat(), cv::Point(-1, -1), 10);

		// Find contours
		findContours(canvas, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

		// Draw contours
		canvas = Mat::zeros(canvas.size(), CV_8UC3);
		for (int i = 0; i< contours.size(); i++) {
			Scalar color = Scalar(255, 255, 255); // THIS DOES RANDOM COLORS - rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			drawContours(canvas, contours, i, color, -10, 8, hierarchy, 0, Point());
		}

		//Build blob detector
		SimpleBlobDetector detector(params);

		// Detect blobs
		detector.detect(canvas, keypoints);

		// Draw detected blobs as red circles.
		// DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures
		// the size of the circle corresponds to the size of blob
		drawKeypoints(canvas, keypoints, canvas, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);


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


VisionProcessor::~VisionProcessor()
{
}
