#include"ObjectLocator.hpp"

ObjectLocator::ObjectLocator(double contour_threshold) {
	this->contour_threshold = contour_threshold;
}

std::vector<cv::Rect> ObjectLocator::DetectObjects(cv::Mat &frame) {
	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(frame,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
	auto boundingBoxes = this->GetBoundingBoxFromContour(contours);
	return boundingBoxes;
}


std::vector<cv::Rect> ObjectLocator::GetBoundingBoxFromContour(
						std::vector<std::vector<cv::Point>> contours) {
	std::vector<cv::Rect> rects_from_countour;
	for (auto contour: contours) {
		if (cv::contourArea(contour) >= this->contour_threshold) {
			cv::Rect rec= cv::boundingRect(contour);
			rects_from_countour.push_back(rec);
		}
	}
	return rects_from_countour;   
}

