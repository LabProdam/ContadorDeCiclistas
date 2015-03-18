#include "ObjectTracker.hpp"
#include "PointTracker.hpp"
#include <time.h>

ObjectTracker::ObjectTracker(unsigned int distanceThreshold, double contour_threshold, cv::Rect interest_area) {
    this->distance_threshold = distanceThreshold;
    this->object_locator = new ObjectLocator(contour_threshold);
    this->object_counter = new ObjectCounter(interest_area);
}

void ObjectTracker::IterateTracker(cv::Mat &frame, cv::Mat &fore) {
    this->CreateNewTracker();
    this->AddRectanglesToTracker(frame, fore);
    this->AddDate(frame);
    this->PrintCounters(frame);  
    this->AccountNewObjects(frame);
    this->RenewTrackers();
}

void ObjectTracker::SetInterestArea(cv::Rect interest_area) {
    this->object_counter->SetReferenceBox(interest_area);
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

void ObjectTracker::AddDate(cv::Mat &frame) {
    time_t time_now = time(0);
    char buff[50];
    int yPos = 15;
	    
    strftime(buff, sizeof(buff), "%d/%m/%Y", localtime(&time_now));
    cv::putText(frame, std::string(buff), cv::Point(2, yPos), CV_FONT_HERSHEY_PLAIN, 1, cv::Scalar(255, 255, 255), 2, CV_AA);
    cv::putText(frame, std::string(buff), cv::Point(2, yPos), CV_FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 0, 0), 1, CV_AA);

    strftime(buff, sizeof(buff), "%X", localtime(&time_now));
    cv::Size sz = cv::getTextSize(std::string(buff), CV_FONT_HERSHEY_PLAIN, 1, 2, NULL);
    cv::putText(frame, std::string(buff), cv::Point(frame.size().width - sz.width - 2, yPos), CV_FONT_HERSHEY_PLAIN, 1, cv::Scalar(255, 255, 255), 2, CV_AA);
    cv::putText(frame, std::string(buff), cv::Point(frame.size().width - sz.width - 2, yPos), CV_FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 0, 0), 1, CV_AA);
}

void ObjectTracker::PersistImage(cv::Mat &frame) {
    char fileName[20];
    sprintf(fileName, "tmp/bike%d.jpg", this->image_counter++);
    cv::imwrite( fileName, frame );
}

void ObjectTracker::AccountNewObjects(cv::Mat &frame) {
    static int imCounter = 0;
    if (this->old_tracker && this->new_tracker) {
	for(auto trackedRect: this->new_tracker->points) {
	    auto rectCenter = trackedRect.second;
	    if(object_counter->AccountPoint(rectCenter)) {
		this->PersistImage(frame);
	    }
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
    sprintf(id, "Total: %02d", object_counter->GetTotalPoints());
    cv::putText(frame, std::string(id),  cv::Point(2, 30), CV_FONT_HERSHEY_PLAIN, 1, cv::Scalar(255, 255, 255), 2, CV_AA);
    cv::putText(frame, std::string(id),  cv::Point(2, 30), CV_FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 0, 0), 1, CV_AA);	
    
    sprintf(id, ">%02d", object_counter->GetLTRPoints());
    cv::putText(frame, std::string(id),  cv::Point(2, 60), CV_FONT_HERSHEY_PLAIN, 1, cv::Scalar(255, 255, 255), 2, CV_AA);
    cv::putText(frame, std::string(id),  cv::Point(2, 60), CV_FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 0, 0), 1, CV_AA);
    
    sprintf(id, "<%02d", object_counter->GetRTLPoints());
    cv::putText(frame, std::string(id),  cv::Point(2, 45), CV_FONT_HERSHEY_PLAIN, 1, cv::Scalar(255, 255, 255), 2, CV_AA);
    cv::putText(frame, std::string(id),  cv::Point(2, 45), CV_FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 0, 0), 1, CV_AA);
}

void ObjectTracker::PrintTotal(cv::Mat &frame) {
    char id[10];
    cv::Point pt(10, 50);
    sprintf(id, "Total: %02d", object_counter->GetTotalPoints());
    cv::putText(frame, std::string(id),  pt, CV_FONT_HERSHEY_PLAIN, 3, cv::Scalar(0, 0, 0), 5, CV_AA);
    cv::putText(frame, std::string(id),  pt, CV_FONT_HERSHEY_PLAIN, 3, cv::Scalar(0, 255, 255), 3, CV_AA);
}

void ObjectTracker::PrintRightPartial(cv::Mat &frame, cv::Point pt) {
    char id[10];
    sprintf(id, ">%02d", object_counter->GetLTRPoints());
    cv::putText(frame, std::string(id),  pt, CV_FONT_HERSHEY_PLAIN, 3, cv::Scalar(0, 0, 0), 5, CV_AA);
    cv::putText(frame, std::string(id),  pt, CV_FONT_HERSHEY_PLAIN, 3, cv::Scalar(0, 255, 255), 3, CV_AA);
}

void ObjectTracker::PrintLeftPartial(cv::Mat &frame, cv::Point pt) {
    char id[10];
    sprintf(id, "<%02d", object_counter->GetRTLPoints());
    cv::putText(frame, std::string(id),  pt, CV_FONT_HERSHEY_PLAIN, 3, cv::Scalar(0, 0, 0), 5, CV_AA);
    cv::putText(frame, std::string(id),  pt, CV_FONT_HERSHEY_PLAIN, 3, cv::Scalar(0, 255, 255), 3, CV_AA);
}
