#include "PointTracker.hpp"

int PointTracker::DistanceInThreshold(cv::Point &a, cv::Point &b) {
	int retVal = 0;
	if (fabs(a.x - b.x) <= maximum_threshold &&
		fabs(a.y - b.y) <= maximum_threshold) {

		if (b.x > a.x) {
			retVal = 1;
		}
		else {
			retVal = -1;
		}
	}
	return retVal;
}

PointTracker::PointTracker(unsigned int threshold) {
	this->maximum_threshold = threshold;
	this->reference = NULL;
}

PointTracker::PointTracker(unsigned int threshold, PointTracker reference) {
	this->maximum_threshold = threshold;
	this->reference = new PointTracker();
	*(this->reference) = reference;
}

unsigned int PointTracker::absolutePointNumber = 0;

void PointTracker::AddPoint(cv::Point &point, cv::Rect &rectangle) {
	if (this->reference) {
		for (auto refpt: reference->points) {
			auto oit = refpt.second;
			int withinThreshold = this->DistanceInThreshold(oit.pt, point);
			if (withinThreshold) {
				bool ltr = withinThreshold == 1;
				this->points[oit.id].pt = point;
				this->points[oit.id].pt0 = oit.pt0;
				this->points[oit.id].rect = rectangle;
				this->points[oit.id].id = oit.id;
				this->points[oit.id].ltr = ltr;
				this->points[oit.id].framesAlive = oit.framesAlive + 1;
				return;
			}			
		}
	}
	this->points[absolutePointNumber] = TrackedObject(point, rectangle,
													  absolutePointNumber, 0);
	absolutePointNumber++;
}

cv::Point RectangleTracker::GetRectangleCenter(cv::Rect &rec) {
	cv::Point tr = rec.tl();
	cv::Point br = rec.br();

	cv::Point center;
	center.x = (tr.x + br.x) / 2.0;
	center.y = (tr.y + br.y) / 2.0;

	return center;
}

RectangleTracker::RectangleTracker(unsigned int threshold):
				  PointTracker(threshold) {}
RectangleTracker::RectangleTracker(unsigned int threshold,
					PointTracker reference):
				  PointTracker(threshold, reference) {}

void RectangleTracker::AddRectangle(cv::Rect &rectangle) {
	cv::Point gravity_center = this->GetRectangleCenter(rectangle);
	this->AddPoint(gravity_center, rectangle);
}	

