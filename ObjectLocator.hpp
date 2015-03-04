#ifndef OBJECT_LOCATOR__H
#define OBJECT_LOCATOR__H

#include<opencv2/opencv.hpp>

class ObjectLocator {
private:
    double contour_threshold;
    
public:
    ObjectLocator(double contour_threshold);
    std::vector<cv::Rect> DetectObjects(cv::Mat &frame);

protected:    
    std::vector<cv::Rect> GetBoundingBoxFromContour(std::vector<std::vector<cv::Point>> contours);
};

#endif
