#ifndef OBJECT_LOCATOR_HPP
#define OBJECT_LOCATOR_HPP

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
