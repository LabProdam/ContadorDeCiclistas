#include "ObjectTracker.hpp"
#include "PointTracker.hpp"

ObjectTracker::ObjectTracker(unsigned int distanceThreshold, double contour_threshold, cv::Rect interest_area) {
    this->distance_threshold = distanceThreshold;
    this->object_locator = new ObjectLocator(contour_threshold);
    this->object_counter = new ObjectCounter(interest_area);
}

void ObjectTracker::IterateTracker(cv::Mat &frame, cv::Mat &fore) {
    this->CreateNewTracker();
    this->AddRectanglesToTracker(frame, fore);
    this->AccountNewObjects(frame);
    this->RenewTrackers();
    this->PrintCounters(frame);  
}

void ObjectTracker::CreateNewTracker() {
    if (this->old_tracker) {
	this->new_tracker = new RectangleTracker(this->distance_threshold, *(this->old_tracker));				
    }
    else {		
	this->new_tracker = new RectangleTracker(this->distance_threshold);
    }			
}

void ObjectTracker::AddRectanglesToTracker(cv::Mat &frame, cv::Mat &fore) {
    auto rects_from_countour = object_locator->DetectObjects(fore);
    for (auto rec: rects_from_countour) {
	    this->new_tracker->AddRectangle(rec);					
	    cv::rectangle(frame, rec, cv::Scalar(0, 0, 255), 1);
    }
}

void ObjectTracker::PersistImage(cv::Mat &frame) {
    char fileName[20];
    sprintf(fileName, "tmp/bike%d.jpg", this->image_counter++);
    cv::imwrite( fileName, frame );
}

void ObjectTracker::AccountNewObjects(cv::Mat &frame) {
    if (this->old_tracker && this->new_tracker) {
	for(auto trackedRect: this->new_tracker->points) {
	    auto rectCenter = trackedRect.second;
	    object_counter->AccountPoint(rectCenter);
	    //TODO
	    char id[5];
	    sprintf(id, "%02d", rectCenter.id);
	    cv::putText(frame, std::string(id),  rectCenter.pt, CV_FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 255, 0));
	}
	delete old_tracker;
	old_tracker = NULL;
    }
}

void ObjectTracker::RenewTrackers() {
    if (new_tracker) {
	old_tracker = new_tracker;
    }  
}

void ObjectTracker::PrintCounters(cv::Mat &frame) {
    char id[10];
    sprintf(id, "%02d", object_counter->GetTotalPoints());
    cv::putText(frame, std::string(id),  cv::Point(2, 15), CV_FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 0, 0));	
    
    sprintf(id, ">%02d", object_counter->GetLTRPoints());
    cv::putText(frame, std::string(id),  cv::Point(2, 30), CV_FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 0, 0));				
    
    sprintf(id, "<%02d", object_counter->GetRTLPoints());
    cv::putText(frame, std::string(id),  cv::Point(2, 45), CV_FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 0, 0));			   
}
