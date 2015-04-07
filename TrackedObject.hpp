#ifndef TRACKED_OBJECT__H
#define TRACKED_OBJECT__H

#include<opencv2/opencv.hpp>

class TrackedObject {
public:
	cv::Point pt;
	cv::Point pt0;
	cv::Rect rect;
	unsigned int id;
	unsigned int framesAlive;
	bool ltr; //Left to right direction

	TrackedObject();	
	TrackedObject(cv::Point pt, cv::Rect rectangle,unsigned int id,
			bool left2right);
};
#endif
