#include<opencv2/opencv.hpp>
#include<algorithm>
#include<iostream>
#include<vector>
#include<map>
#include<getopt.h>
#include<unistd.h>
#include<fcntl.h>

#include "Opt.hpp"
#include "Utils.hpp"
#include "ImageProcessor.hpp"
#include "ObjectTracker.hpp"
#include "VideoOutput.hpp"
#include "Sensors.hpp"

unsigned int x_counter[2];
unsigned int y_counter[2];

unsigned int x[4];
unsigned int y[4];

unsigned int x_crop[2];
unsigned int y_crop[2];

unsigned int x_interest[2];
unsigned int y_interest[2];

void LoadConfiguration() {
	Config cfg;
	
	for (int i = 0; i < sizeof(x_counter) / sizeof(x_counter[0]); i++) {
		cv::Point pt = cfg.GetCounterPos(i);
		x_counter[i] = pt.x;
		y_counter[i] = pt.y;
	}
	
	for (int i = 0; i < sizeof(x) / sizeof(x[0]); i++) {
		cv::Point pt = cfg.GetPerspectivePos(i);
		x[i] = pt.x;
		y[i] = pt.y;
	}
	
	for (int i = 0; i < sizeof(x_crop) / sizeof(x_crop[0]); i++) {
		cv::Point pt = cfg.GetCropPos(i);
		x_crop[i] = pt.x;
		y_crop[i] = pt.y;
	}
	
	for (int i = 0; i < sizeof(x_interest) / sizeof(x_interest[0]); i++) {
		cv::Point pt = cfg.GetInterestPos(i);
		x_interest[i] = pt.x;
		y_interest[i] = pt.y;
	}	
}

void SaveConfiguration() {
	Config cfg;
	
	for (int i = 0; i < sizeof(x_counter) / sizeof(x_counter[0]); i++) {
		cv::Point pt;
		pt.x = x_counter[i];
		pt.y = y_counter[i];
		cfg.SetCounterPos(i, pt);
	}
	
	for (int i = 0; i < sizeof(x) / sizeof(x[0]); i++) {
		cv::Point pt;
		pt.x = x[i];
		pt.y = y[i];
		cfg.SetPerspectivePos(i, pt);		
	}
	
	for (int i = 0; i < sizeof(x_crop) / sizeof(x_crop[0]); i++) {
		cv::Point pt;
		pt.x = x_crop[i];
		pt.y = y_crop[i];
		cfg.SetCropPos(i, pt);
	}
	
	for (int i = 0; i < sizeof(x_interest) / sizeof(x_interest[0]); i++) {
		cv::Point pt;
		pt.x = x_interest[i];
		pt.y = y_interest[i];
		cfg.SetInterestPos(i, pt);		
	}	
}

enum InteractionAction {
        SET_COUNTERS_AREA,
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
		case SET_COUNTERS_AREA:
                    CurrentCallbackFunction = GetCountersArea;
		break;
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
 
        static void GetCountersArea(int x, int y) {
            static int step = 0;
            static int x_internal[2];
            static int y_internal[2];
            x_internal[step] = x;
            y_internal[step] = y;
            switch (step) {
                case 0:
                    printf("Ponto %d %d\nSelecione Ponto de Contador a Direita\n");
                    break;
                case 1:              
                    step = 0;
                    for (int i = 0; i < sizeof(::x_counter)/sizeof(::x_counter[0]); i++) {
                            ::x_counter[i] = x_internal[i];
                            ::y_counter[i] = y_internal[i];
                    }
                    printf("Selecione Ponto de Perspectiva Inicial\n");
                    InteractionHandler::SetAction(InteractionAction::SET_PERSPECTIVE_AREA);
                    return;                    
            }
            step++;
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
		    SaveConfiguration();
                    InteractionHandler::SetAction(InteractionAction::NONE); 		    
                    return;                    
            }
            step++;
        }
};

void (*InteractionHandler::CurrentCallbackFunction)(int x, int y) = NULL;


//TESTE !!! inicio
#include "CoordTransform.hpp"
#include "Camera.hpp"
#define PI (4 * atan(tan(1.)))
void initCamera(cv::Mat& f) {
	Camera cam;
	cam.SetFrameRows(f.rows);
	cam.SetFrameCols(f.cols);
	cv::Point2f A, B;
	A.x = 362; A.y = 86;  //ponto de cima da trena  (foto em que estou na parede)
	B.x = 364; B.y = 225; //ponto de baixo da trena (foto em que estou na parede)
	float distTrenaCamera = 6.;
	float tamTrena = 1.;
	//nao e' necessario mudar as coordenadas de A e B pois o metodo abaixo usa a
	//distancia entre A e B.
	cam.SetFocalDistance(distTrenaCamera, tamTrena, A, B);
	std::cout << "focal distance " << cam.GetFocalDistance() << "px" << std::endl;
	A.x = 389; A.y = 194;
	B.x = 388; B.y = 228;
	float coquinhoAteX = 1.43;
	//nao e' necessario mudar as coordenadas de A e B pois o metodo abaixo usa a
	//distancia entre A e B.
	cam.SetHeight(coquinhoAteX, A, B);
	std::cout << "cam height " << cam.GetHeight() << "m" << std::endl;
	A.x = 219; A.y = 275; //top cabo vassoura (top, left)
	B.x = 217; B.y = 313; //top cabo vassoura (bottom, left)
	float l = 2.32; //l = Rz1 - Rz2 = largura ciclofaixa;
	float tamCaboVassoura = 1.17;
	cam.SetTheta(l, A, B, tamCaboVassoura);
	std::cout << "theta " << cam.GetTheta()*180./PI << "°" << std::endl;
	A.y = 275;
	cam.SetRz(A, tamCaboVassoura);
	std::cout << "Rz " << cam.GetRz() << std::endl;
	float asd, zxc;
	cv::Point2f PP1;
	cv::Point2f PP2;
	PP1.x = 248; PP1.y = 55; //semaforo top
	PP2.x = 251; PP2.y = 90; //semafoto bottom
	asd = CoordTransform::GetRealRyFromImageiPoint(cam, PP1);
	zxc = CoordTransform::GetRealRyFromImageiPoint(cam, PP2);
	std::cout << "h1 = " << asd << " . h2 = " << zxc << ".... |h1-h2| = " << fabs(asd - zxc) << std::endl;
	PP1.y = 275; //vassoura top
	PP2.y = 288; //vassoura bottom
	std::cout << "tam T: " << (asd = CoordTransform::GetRealRyFromImageiPoint(cam, PP1)) << std::endl;
	std::cout << "tam B: " << (zxc = CoordTransform::GetRealRyFromImageiPoint(cam, PP2)) << std::endl;
	std::cout << "vassoura tem: " << asd - zxc << std::endl;
	PP1.y = 134; //poste grande top
	PP2.y = 263; //poste grande bottom
	std::cout << "tam T: " << (asd = CoordTransform::GetRealRyFromImageiPoint(cam, PP1)) << std::endl;
	std::cout << "tam B: " << (zxc = CoordTransform::GetRealRyFromImageiPoint(cam, PP2)) << std::endl;
	std::cout << "poste grande tem: " << asd - zxc << std::endl;
	std::string fn(".camera.conf");
	cam.SaveConf(fn);
}
void tt(void)
{
	std::string a(".camera.conf");
	std::string b(".cameraa.conf");
	Camera cam(a);
	Camera cam2(b);
	cv::Point2f PP1, PP2;
	float asd, zxc;
	std::cout << "focal distance " << cam.GetFocalDistance() << "px" << std::endl;
	std::cout << "cam height " << cam.GetHeight() << "m" << std::endl;
	std::cout << "theta " << cam.GetTheta()*180./PI << "°" << std::endl;
	std::cout << "Rz " << cam.GetRz() << std::endl;
	PP1.y = 134; //poste grande top
	PP2.y = 263; //poste grande bottom
	std::cout << "tam T: " << (asd = CoordTransform::GetRealRyFromImageiPoint(cam, PP1)) << std::endl;
	std::cout << "tam B: " << (zxc = CoordTransform::GetRealRyFromImageiPoint(cam, PP2)) << std::endl;
	std::cout << "poste grande tem: " << asd - zxc << std::endl;
}
//TESTE !!! fim


#define DEV_FILE 0x02
#define REG_FILE 0x01
#define UNK_FILE 0x00
int main(int argc, char **argv) {

    static struct option long_options[] = {
    	{ "reg_source", required_argument, NULL, 's'},
    	{ "override", 	required_argument, NULL, 'O'},
    	{ "dev_source", required_argument, NULL, 'D'},
    	{ "record",     required_argument, NULL, 'r'},
    	{ "stream",     required_argument, NULL, 'S'},
    	{ "address",    required_argument, NULL, 'a'},
    	{ "sensor",     required_argument, NULL, 't'},
    	{ "help",       no_argument,       NULL, 'h'},
    	{ 0,        0,                 0,    0}
    };
    int opt = 0;
    int long_index = 0;
    
    std::string source_file    = ""; //where data comes from if source is a regular file.
    int  source_device         = -1; //where data comes from if source is device file.
    std::string record_file    = ""; //output file.
    std::string stream_device  = ""; //device file used to stream data.
    std::string address        = ""; //address where program is running on.
    std::string sensor_device  = ""; //sensor device file.
    bool pick_points = false;
    bool help = false;
    bool abort = false;
    int source = UNK_FILE;

    while((opt = getopt_long(argc, argv, "s:r:S:t:a:D:hO", long_options, &long_index))
    		!= -1) {
    	switch(opt) {
    		case 's':
    			source_file = optarg;
    			source |= REG_FILE;
    			break;
    		case 'D':
    			source_device = atoi(optarg);
    			source |= DEV_FILE;
    			break;
    		case 'r':
    			record_file = optarg;
    			break;
    		case 'S':
    			stream_device = optarg;
    			break;
    		case 'a':
    			address = optarg;
    			break;
    		case 't':
    			sensor_device = optarg;
    			break;
    		case 'h':
    			help = true;
    			break;
		case 'O':
    			pick_points = true;
    			break;
    		default:
    			return EXIT_FAILURE;
    			break;
    	}
    }

    if(help) {
    	print_usage((std::string) argv[0]);
    	return EXIT_SUCCESS;
    }

    if(address.empty()) {
    	std::cout << "You must specify your address. "
    		<< "Like --address faria_lima-1280." << std::endl;
    	abort = true;

    }

    if(source == (REG_FILE | DEV_FILE)) {
    	std::cout << "You can only have one source. It has to be either a regular file " <<
    		"or a device file, not both." << std::endl;
    	abort = true;
    }

    if(source == UNK_FILE) {
    	std::cout << "You must specify where data comes from." << std::endl <<
    		"\tUse --source <source_file>. Or --help to print usage." <<
    		std::endl;
    	abort = true;
    }

    if( ((source & REG_FILE) && (source_file == record_file))   ||
    	((source & REG_FILE) && (source_file == stream_device)) ||
    	((record_file == stream_device) && !record_file.empty())
    	) {
    	std::cout << "Files must not be the same." << std::endl;
    	abort = true;
    }

    if(abort)
    	return EXIT_FAILURE;

    //Check all files forced.
    if(source == REG_FILE)
    	abort = !test_file(source_file);
    abort = !test_file(record_file)   || abort;
    abort = !test_file(stream_device) || abort;
    abort = !test_file(sensor_device) || abort;

    if(abort)
    	return EXIT_FAILURE;


    int fdwr = 0;
    int ret_code = 0;
    const char *c = NULL;
    cv::VideoWriter *output = NULL;
    VideoOutput *outputDevice = NULL;
    std::thread *sensorsThread = NULL;
    SensorData sd;

    cv::Mat frame;
    cv::Mat full;
    cv::Mat fore;
    cv::VideoCapture *cap = NULL;
	
    if(source == REG_FILE)
    	cap = new cv::VideoCapture(source_file);
    else
    	cap = new cv::VideoCapture(source_device);
    if(!cap) {
    	std::cout << "Error: could not create a VideoCapture object" << std::endl;
    	exit(EXIT_FAILURE);
    }

    if(!stream_device.empty()) {
    	c = stream_device.c_str();
        outputDevice = new VideoOutput(c);
    }

    if(!sensor_device.empty()) {
    	c = sensor_device.c_str();
    	sensorsThread = StartSensorsThread(c, &sd);
    }

    ImageProcessor ip;   
    
    *cap >> frame;
    cv::Size frame_size = frame.size();

    if(!record_file.empty())
	output = new cv::VideoWriter(record_file, CV_FOURCC('M', 'P', 'E', 'G'), 30, frame_size);

    memset(x_counter, 0, sizeof(x_counter));
    memset(y_counter, 0, sizeof(y_counter));
    
    if (!pick_points) {
	LoadConfiguration();
    }
    
    //x[0] = 0                ; y[0] = 0;
    x[1] = x[1]?x[1]:frame_size.width ; //y[1] = 0;
    /*x[2] = 0                ; */ y[2] = y[2]?y[2]:frame_size.height;
    x[3] = x[3]?x[3]:frame_size.width ; y[3] = y[3]?y[3]:frame_size.height;
   
    //x_crop[0] = 0                 ; y_crop[0] = 0;
    x_crop[1] = x_crop[1]?x_crop[1]:frame_size.width  ; y_crop[1] = y_crop[1]?y_crop[1]:frame_size.height;
    
    //x_interest[0] = 0; y_interest[0] = 0;
    //x_interest[1] = 1; y_interest[1] = 1;    
      
    int imageNum = 0;
    
    cv::imshow(address, frame);    
        
    if (pick_points) {
	    InteractionHandler::Subscribe(address);
		
	    printf("Selecione Ponto de Contador a Esquerda\n");
	    InteractionHandler::SetAction(InteractionAction::SET_COUNTERS_AREA);
    }
    cv::Rect interestArea(x_interest[0], y_interest[0], x_interest[1] - x_interest[0], y_interest[1] - y_interest[0]);
    ObjectTracker ot(30, 50, interestArea);

    while(1) {
		cv::Point lCounter(x_counter[0], y_counter[0]);
		cv::Point rCounter(x_counter[1], y_counter[1]);

		cv::Point2f p0(x[0], y[0]);
		cv::Point2f p1(x[1], y[1]);
		cv::Point2f p2(x[2], y[2]);
		cv::Point2f p3(x[3], y[3]); 
		
		cv::Rect cropFrame(x_crop[0], y_crop[0], x_crop[1] - x_crop[0], y_crop[1] - y_crop[0]); 
		interestArea = cv::Rect(x_interest[0], y_interest[0], x_interest[1] - x_interest[0], y_interest[1] - y_interest[0]);
		ot.SetInterestArea(interestArea);
		
		*cap >> frame;
		if(!record_file.empty())
			output->write(frame); //Write avi file
			
		full = frame.clone();
		ip.PrepareFrame(frame, cropFrame, p0, p1, p2, p3);
		fore = ip.AcquireForeground(frame);
		ip.InsertInterestArea(frame, interestArea);		
		ot.IterateTracker(frame, fore);
		ot.PrintTotal(full);
		ot.PrintLeftPartial(full, lCounter);
		ot.PrintRightPartial(full, rCounter);
		
		ProvideOsd(full, sensor_device.empty()?NULL : &sd, ot);
		ProvidePip(frame, full);		  
		
		cv::imshow(address, frame);		
		cv::imshow("Full Frame", full);
		
		if(!stream_device.empty())
			outputDevice->write(full);

		if(cv::waitKey(30) == 27)
			break;
			
		if (IsMidnight()) {
			ot.ZeroCounters();
		}
		
    }
   
    if(!sensor_device.empty()) {
        sensorsThread->join();
    }

    delete cap;
    if(outputDevice)
    	delete outputDevice;
    if(output)
    	delete output;
	
    return 0;
}
