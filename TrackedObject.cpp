#include "TrackedObject.hpp"

TrackedObject::TrackedObject() {
	this->id = 0;
	this->ltr = 0;
	this->framesAlive = 0;	
}

TrackedObject::TrackedObject(cv::Point pt, unsigned int id, bool left2right) {
	this->pt = pt;
	this->id = id;
	this->ltr = left2right;
	this->framesAlive = 0;
}
