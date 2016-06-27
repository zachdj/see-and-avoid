#include "BlobTracker.h"

struct TempBlobInfo {
	double firstOccurrenceX, lastOccurrenceX;
	double firstOccurrenceY, lastOccurrenceY;
	double firstOccurrenceSize, lastOccurrenceSize;
	double cumulativeCollisionValue;
	double totalOccurrences;
};

BlobTracker::BlobTracker()
{
	this->historyLength = 10;
	this->history = vector<vector<TrackedBlob> >(this->historyLength);
}

BlobTracker::BlobTracker(unsigned int historyLength)
{
	this->historyLength = historyLength;
	this->history = vector<vector<TrackedBlob> >(this->historyLength);
}

void BlobTracker::AddFrame(vector<cv::KeyPoint> keypoints) {
	vector<TrackedBlob> blobs; // represents the blobs found at the current frame
	vector<TrackedBlob> lastFrame = vector<TrackedBlob>(0);
	if (this->history.size() > 0) {
		lastFrame = this->history[0];
	}
	for (int i = 0; i < keypoints.size(); i++) {
		cv::KeyPoint currentKeypoint = keypoints[i];
		unsigned int newBlobId = 0;

		//determine if current keypoint matches a keypoint from the previous frame
		for (int j = 0; j < lastFrame.size(); j++) {
			TrackedBlob lastFramesBlob = lastFrame[j];
			//if distance is less than 60...
			if (pointToPointDistance2(lastFramesBlob, currentKeypoint) < pow(60, 2)) {
				// .. then we've found a match!
				newBlobId = lastFramesBlob.GetId();
			}
		}

		// create a new tracked blob for this keypoint
		if (newBlobId == 0) { //if a match wasn't found
			double yVal = currentKeypoint.pt.y;
			blobs.insert(blobs.begin(), TrackedBlob(currentKeypoint.pt.x, yVal, currentKeypoint.size));
		} else {
			double yVal = currentKeypoint.pt.y;
			blobs.insert(blobs.begin(), TrackedBlob(newBlobId, currentKeypoint.pt.x, yVal , currentKeypoint.size));
		}
	}

	this->history.insert(this->history.begin(), blobs);
	
	//truncate history if necessary
	if (this->history.size() > this->historyLength) {
		this->history.resize(this->historyLength);
	}
}

//returns a list of blob info
vector<BlobInfo> BlobTracker::GetBlobInfo(cv::Point center) {
	//map will be indexed by blob id - holds information collected about that blob up to this point
	std::map<unsigned int, TempBlobInfo> blobInfoMap;
	
	//iterate through each level of history, starting with the most recent frame
	//for (int level = 0; level < this->history.size(); level++) {
	for (int level = this->history.size()-1; level >= 0; level--) {
		vector<TrackedBlob> currentLevel = this->history[level];
		//iterate through each blob in this level
		for (int i = 0; i < currentLevel.size(); i++) {
			TrackedBlob currentBlob = currentLevel[i];

			// determine the blob's collision weight at this point
			int distance = pointToPointDistance2(currentBlob.posX, currentBlob.posY, center.x, center.y);
			double weight = 10 / (1 + exp(-0.00009 * (-distance + 20000)));

			//if our map already contains this blob, then just update its info
			if (blobInfoMap.find(currentBlob.GetId()) != blobInfoMap.end()) {
				blobInfoMap[currentBlob.GetId()].lastOccurrenceSize = currentBlob.size;
				blobInfoMap[currentBlob.GetId()].lastOccurrenceX = currentBlob.posX;
				blobInfoMap[currentBlob.GetId()].lastOccurrenceY = currentBlob.posY;
				blobInfoMap[currentBlob.GetId()].cumulativeCollisionValue += weight;
				blobInfoMap[currentBlob.GetId()].totalOccurrences++;
			}
			// otherwise insert the new TempBlobInfo
			else {
				TempBlobInfo newBlobInfo;
				newBlobInfo.firstOccurrenceSize = currentBlob.size;
				newBlobInfo.firstOccurrenceX = currentBlob.posX;
				newBlobInfo.firstOccurrenceY = currentBlob.posY;
				newBlobInfo.lastOccurrenceSize = currentBlob.size;
				newBlobInfo.lastOccurrenceX = currentBlob.posX;
				newBlobInfo.lastOccurrenceY = currentBlob.posY;
				newBlobInfo.cumulativeCollisionValue = weight;
				newBlobInfo.totalOccurrences = 1;

				//blobInfoMap[currentBlob.GetId()] = newBlobInfo;
				blobInfoMap.insert(pair<unsigned int, TempBlobInfo>(currentBlob.GetId(), newBlobInfo));
			}
		}
	}

	int sizeOfMap = blobInfoMap.size();

	//create  BlobInfo object from the combined information in each element of the map
	vector<BlobInfo> blobInfo;
	for (auto const& obj : blobInfoMap)
	{
		BlobInfo info = BlobInfo(obj.first,
			obj.second.lastOccurrenceX - obj.second.firstOccurrenceX,
			obj.second.lastOccurrenceY - obj.second.firstOccurrenceY,
			obj.second.lastOccurrenceSize - obj.second.firstOccurrenceSize,
			(float)obj.second.totalOccurrences / this->historyLength); 
		info.originalPositionX = obj.second.firstOccurrenceX;
		info.originalPositionY = obj.second.firstOccurrenceY;
		info.currentPositionX = obj.second.lastOccurrenceX;
		info.currentPositionY = obj.second.lastOccurrenceY;
		info.currentSize = obj.second.lastOccurrenceSize;
		info.SetCollisionValue(obj.second.cumulativeCollisionValue);

		blobInfo.push_back(info);
	}

	return blobInfo;
}

double BlobTracker::pointToPointDistance2(double x1, double y1, double x2, double y2)
{
	return pow(x1 - x2, 2) + pow(y1 - y2, 2);
}

double BlobTracker::pointToPointDistance2(TrackedBlob blob, cv::KeyPoint keypoint)
{
	double dist = pow(blob.posX - keypoint.pt.x, 2) + pow(blob.posY - keypoint.pt.y, 2);
	return pow(blob.posX - keypoint.pt.x, 2) + pow(blob.posY - keypoint.pt.y, 2);
}

BlobTracker::~BlobTracker()
{
}
