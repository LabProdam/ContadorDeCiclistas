#ifndef IMAGE_PROCESSOR__H
#define IMAGE_PROCESSOR__H

#include<opencv2/opencv.hpp>
#include<iostream>

class ImageProcessor {
private:
	cv::BackgroundSubtractorMOG2 bg;
public:
	ImageProcessor();	
	~ImageProcessor();	
	cv::Mat AcquireForeground(cv::Mat &frame);	
	void PrintText(cv::Mat &frame, std::string text, cv::Point position);
	void InsertInterestArea(cv::Mat &frame,  cv::Rect interestArea);
	void PrepareFrame(cv::Mat &frame, 
					  cv::Rect cropArea, 
					  cv::Point2f p0, 
					  cv::Point2f p1, 
					  cv::Point2f p2, 
					  cv::Point2f p3);
	  
	void RotateImage(cv::Mat &frame);
protected:
	void PerspectiveTransform(cv::Mat &frame, 
							  cv::Point2f p0, 
							  cv::Point2f p1, 
							  cv::Point2f p2, 
							  cv::Point2f p3);
	
	void CropImage(cv::Mat &frame, cv::Rect cropArea);
};
#endif
