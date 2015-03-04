#include<opencv2/opencv.hpp>
#include<algorithm>
#include<iostream>
#include<vector>
#include<map>

#include "ImageProcessor.hpp"
#include "ObjectTracker.hpp"

int main(int argc, char **argv) {
    cv::Mat frame;
    cv::Mat fore;
    cv::VideoCapture cap(0);
    cv::Rect interestArea(80, 10, 40, 70);

    int x0,x1,x2,x3,y0,y1,y2,y3;
    x0=0;y0=273;
    x1=478;y1=309;
    x2=0;y2=370;
    x3=477;y3=386;
    
    cv::Rect cropFrame(0, 0, 478 ,(y2 - y0));    
    cv::Point2f p0(x0, y0);
    cv::Point2f p1(x1, y1);
    cv::Point2f p2(x2, y2);
    cv::Point2f p3(x3, y3); 
    
    ImageProcessor ip;
    ObjectTracker ot(80, 200, interestArea);
    
    cap >> frame;
    cap >> frame;
   
    while(1) {
        cap >> frame;
        ip.PrepareFrame(frame, cropFrame, p0, p1, p2, p3);
        fore = ip.AcquireForeground(frame);
        ip.InsertInterestArea(frame, interestArea);
        cv::imshow("Fore", fore);
        ot.IterateTracker(frame, fore);
        
        if(cv::waitKey(30) == 27) {
				break;
        }
        
        cv::imshow("Faria Lima", frame);
    }
    
    
    
}
