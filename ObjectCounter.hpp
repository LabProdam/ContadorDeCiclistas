#ifndef OBJECT_COUNTER__H
#define OBJECT_COUNTER__H

#include <opencv2/opencv.hpp>
#include "TrackedObject.hpp"

class ObjectCounter {
private:
	unsigned int countedPoint[100];
	unsigned short int pos;
	unsigned int totalLeftCount;
	unsigned int totalRightCount;
	cv::Rect referenceBox;
	
public:
	ObjectCounter(cv::Rect referenceBox);		
	bool AccountPoint(TrackedObject point);		
	unsigned int GetTotalPoints();		
	unsigned int GetLTRPoints();
	unsigned int GetRTLPoints();
	void ZeroCounters();
	void SetReferenceBox(cv::Rect referenceBox);
};
#endif
