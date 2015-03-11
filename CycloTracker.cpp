#include<opencv2/opencv.hpp>
#include<algorithm>
#include<iostream>
#include<vector>
#include<map>

#include "ImageProcessor.hpp"
#include "ObjectTracker.hpp"
#include "VideoOutput.hpp"

int x[4];
int y[4];

int x_crop[2];
int y_crop[2];

int x_interest[2];
int y_interest[2];

enum InteractionAction {
        SET_PERSPECTIVE_AREA,
        SET_CROP_AREA,
        SET_INTEREST_AREA,
        NONE
};

class InteractionHandler {
private:
        static void (*CurrentCallbackFunction)(int x, int y);
public:
        static void Subscribe(std::string windowName) {
                cv::setMouseCallback(windowName.c_str(), InteractionHandler::MouseCallback, NULL);
        }

        static void SetAction(InteractionAction action) {
            switch (action) {
                case SET_PERSPECTIVE_AREA:
                    CurrentCallbackFunction = GetPerspectivePoints;
                break;
                case SET_CROP_AREA:
                    CurrentCallbackFunction = GetCropFrame;
                break;
                case SET_INTEREST_AREA:
                    CurrentCallbackFunction = GetInterestArea;
                break;
                case NONE:
                    CurrentCallbackFunction = NULL;
                break;
            }
        }
        
protected:
        static void MouseCallback(int event, int x, int y, int flags, void *data) {
            if (CurrentCallbackFunction) {
                    switch(event) {
                        case(cv::EVENT_LBUTTONDOWN):
                            CurrentCallbackFunction(x, y);
                            break;
                    }
            }
        }       
                
        static void GetPerspectivePoints(int x, int y) {
            static int step = 0;
            static int x_internal[4];
            static int y_internal[4];
            x_internal[step] = x;
            y_internal[step] = y;
            switch (step) {
                case 0:
                    printf("Ponto %d %d\nSelecione 2\n");
                    break;
                case 1:
                    printf("Ponto %d %d\nSelecione 3\n");
                    break;
                case 2:
                    printf("Ponto %d %d\nSelecione 4\n");
                    break;
                case 3:
                    step = 0;
                    for (int i = 0; i < sizeof(::x)/sizeof(::x[0]); i++) {
                            ::x[i] = x_internal[i];
                            ::y[i] = y_internal[i];
                    }
                    printf("Selecione Ponto de Crop Inicial\n");
                    InteractionHandler::SetAction(InteractionAction::SET_CROP_AREA);
                    return;                    
            }
            step++;
        }
        
        static void GetCropFrame(int x, int y) {
            static int step = 0;
            static int x_internal[2];
            static int y_internal[2];
            x_internal[step] = x;
            y_internal[step] = y;
            switch (step) {
                case 0:
                    printf("Ponto %d %d\nSelecione 2\n");
                    break;
                case 1:
                    step = 0;
                    for (int i = 0; i < sizeof(::x_crop)/sizeof(::x_crop[0]); i++) {
                            ::x_crop[i] = x_internal[i];
                            ::y_crop[i] = y_internal[i];
                    }
                    printf("Selecione Ponto de Interesse Inicial\n");
                    InteractionHandler::SetAction(InteractionAction::SET_INTEREST_AREA);
                    return;                    
            }
            step++;
        }
        
         static void GetInterestArea(int x, int y) {
            static int step = 0;
            static int x_internal[2];
            static int y_internal[2];
            x_internal[step] = x;
            y_internal[step] = y;
            switch (step) {
                case 0:
                    printf("Ponto %d %d\nSelecione 2\n");
                    break;
                case 1:
                    step = 0;
                    for (int i = 0; i < sizeof(::x_interest)/sizeof(::x_interest[0]); i++) {
                            ::x_interest[i] = x_internal[i];
                            ::y_interest[i] = y_internal[i];
                    }
                    InteractionHandler::SetAction(InteractionAction::NONE);
                    return;                    
            }
            step++;
        }
};

void (*InteractionHandler::CurrentCallbackFunction)(int x, int y) = NULL;


int main(int argc, char **argv) {
    int fdwr = 0;
	int ret_code = 0;

    cv::Mat frame;
    cv::Mat fore;
    cv::VideoCapture cap(0);
    VideoOutput outputDevice("/dev/video1");
	
    ImageProcessor ip;   
    
    cap >> frame;
    cv::VideoWriter output("out.avi", CV_FOURCC('M', 'P', 'E', 'G'), 30, frame.size());
    cv::Size frame_size = frame.size();
    
    x[0] = 0                ; y[0] = 0;
    x[1] = frame_size.width ; y[1] = 0;
    x[2] = 0                ; y[2] = frame_size.height;
    x[3] = frame_size.width ; y[3] = frame_size.height;
    
    x_crop[0] = 0                 ; y_crop[0] = 0;
    x_crop[1] = frame_size.width  ; y_crop[1] = frame_size.height;
    
    x_interest[0] = 0; y_interest[0] = 0;
    x_interest[1] = 1; y_interest[1] = 1;
    
    //cap >> frame;
    //cv::imwrite( "tmp/fram0.jpg", frame );
    
    int imageNum = 0;
    
    cv::imshow("Faria Lima", frame);    
    InteractionHandler::Subscribe("Faria Lima");
    
    printf("Selecione Ponto de Perspectiva Inicial\n");
    InteractionHandler::SetAction(InteractionAction::SET_PERSPECTIVE_AREA);

    cv::Rect interestArea(x_interest[0], y_interest[0], x_interest[1] - x_interest[0], y_interest[1] - y_interest[0]);
    ObjectTracker ot(80, 200, interestArea);

    while(1) {
        cv::Point2f p0(x[0], y[0]);
        cv::Point2f p1(x[1], y[1]);
        cv::Point2f p2(x[2], y[2]);
        cv::Point2f p3(x[3], y[3]); 
		
        cv::Rect cropFrame(x_crop[0], y_crop[0], x_crop[1] - x_crop[0], y_crop[1] - y_crop[0]); 
        interestArea = cv::Rect(x_interest[0], y_interest[0], x_interest[1] - x_interest[0], y_interest[1] - y_interest[0]);
        ot.SetInterestArea(interestArea);
        
        cap >> frame;
        output.write(frame); //Write avi file
        ip.PrepareFrame(frame, cropFrame, p0, p1, p2, p3);
        fore = ip.AcquireForeground(frame);
        ip.InsertInterestArea(frame, interestArea);
        cv::imshow("Fore", fore);
        ot.IterateTracker(frame, fore);
        
        /*if(cv::waitKey(30)  == 32) {
            imageNum++;
            char fileName[20];
            sprintf(fileName, "tmp/cap%02d.jpg", imageNum);
            cv::imwrite(fileName, frame );
        }*/
        
        cv::imshow("Faria Lima", frame);
		outputDevice.write(frame);
    }    

	return 0;
}
