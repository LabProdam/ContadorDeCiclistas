#ifndef POINT_TRACKER__H
#define POINT_TRACKER__H

#include <opencv2/opencv.hpp>
#include "TrackedObject.hpp"

class PointTracker {
private:
	PointTracker *reference;
	static unsigned int absolutePointNumber;
	unsigned int point_id;
	unsigned int maximum_threshold;
	bool hasReference;
	
	int DistanceInThreshold(cv::Point &a, cv::Point &b);
	
public:
	std::map<int, TrackedObject> points;
	
	PointTracker(unsigned int threshold = 10);	
	PointTracker(unsigned int threshold, PointTracker reference);	
	void AddPoint(cv::Point &point);
};

class RectangleTracker : public PointTracker {
private:
	cv::Point GetRectangleCenter(cv::Rect &rec);

public:		
	RectangleTracker(unsigned int threshold);
	RectangleTracker(unsigned int threshold, PointTracker reference);

	void AddRectangle(cv::Rect &rectangle);
};

#endif
