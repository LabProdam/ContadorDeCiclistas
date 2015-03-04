#ifndef OBJECT_TRACKER__H
#define OBJECT_TRACKER__H

#include <opencv2/opencv.hpp>
#include "TrackedObject.hpp"
#include "ObjectCounter.hpp"
#include "ObjectLocator.hpp"
#include "PointTracker.hpp"

class ObjectTracker { 
private:
	RectangleTracker *old_tracker = NULL;
	RectangleTracker *new_tracker = NULL;
	ObjectLocator *object_locator = NULL;
	ObjectCounter *object_counter;
	unsigned int distance_threshold;
	unsigned int image_counter = 0;
    
public:
	ObjectTracker(unsigned int distanceThreshold, double contour_threshold, cv::Rect interest_area);    
	void IterateTracker(cv::Mat &frame, cv::Mat &fore);

private:
	void CreateNewTracker();    
	void AddRectanglesToTracker(cv::Mat &frame, cv::Mat &fore);
	void PersistImage(cv::Mat &frame);    
	void AccountNewObjects(cv::Mat &frame);
	void RenewTrackers();
	void PrintCounters(cv::Mat &frame);
};

#endif
