#include"ImageProcessor.hpp"

ImageProcessor::ImageProcessor() {
//	this->lambda = new cv::Mat(2, 4, CV_32FC1);
//	this->inv_lambda = new cv::Mat(2, 4, CV_32FC1);
}
ImageProcessor::~ImageProcessor() {}
	
cv::Mat ImageProcessor::AcquireForeground(cv::Mat &frame) {
		cv::Mat fore;
	cv::Mat greyImg;
	//cvtColor( frame, frame, CV_BGR2GRAY );
	//equalizeHist( frame, frame );
		this->bg(frame, fore);
		cv::erode(fore, fore, cv::Mat());
		return fore;
}
	
void ImageProcessor::PrintText(cv::Mat &frame, std::string text,
							   cv::Point position) {
	cv::putText(frame, text,  position, CV_FONT_HERSHEY_PLAIN, 1,
				cv::Scalar(0, 0, 0));
}
	
void ImageProcessor::InsertInterestArea(cv::Mat &frame,cv::Rect interestArea) {
	cv::rectangle(frame, interestArea, cv::Scalar(0, 0, 0), 1);
}

void ImageProcessor::PrepareFrame(cv::Mat &frame, 
		  cv::Rect cropArea, 
		  cv::Point2f p0, 
		  cv::Point2f p1, 
		  cv::Point2f p2, 
		  cv::Point2f p3) {
	//this->RotateImage(frame);
	this->PerspectiveTransform(frame, p0, p1, p2, p3);
	this->CropImage(frame, cropArea);
}


void ImageProcessor::RotateImage(cv::Mat &frame) {
	cv::transpose(frame, frame);
	cv::flip(frame, frame, 0);		
	cv::flip(frame, frame, 1);		
}

void ImageProcessor::PerspectiveTransform(cv::Mat &frame, 
			  cv::Point2f p0, 
			  cv::Point2f p1, 
			  cv::Point2f p2, 
			  cv::Point2f p3) {
				  
	cv::Point2f inputQuad[4]; 
	cv::Point2f outputQuad[4];
	
	inputQuad[0] = p0;
	inputQuad[1] = p1;
	inputQuad[2] = p2;
	inputQuad[3] = p3;			
			
	int x0d, x1d, x2d, x3d, y0d, y1d, y2d, y3d;		 
	x0d=0;    y0d=0;
	x1d=p1.x; y1d=0;
	x2d=0;    y2d=std::max(p2.y - p0.y, p3.y - p1.y);
	x3d=p3.x; y3d=std::max(p2.y -p0.y, p3.y - p1.y);
	
	outputQuad[0] = cv::Point2f(x0d, y0d);
	outputQuad[1] = cv::Point2f(x1d, y1d);
	outputQuad[2] = cv::Point2f(x2d, y2d);
	outputQuad[3] = cv::Point2f(x3d, y3d);


	// Get the Perspective Transform Matrix i.e. lambda
	this->lambda = cv::getPerspectiveTransform(inputQuad, outputQuad);
	// Apply the Perspective Transform just found to the src image
	cv::warpPerspective(frame, frame, this->lambda, frame.size());
	this->frame = frame;
}

void ImageProcessor::CropImage(cv::Mat &frame, cv::Rect cropArea) {
		frame = frame(cropArea);
}
