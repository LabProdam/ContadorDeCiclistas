#include "Utils.hpp"
#include <sys/timeb.h>

bool IsMidnight() {
    time_t rawtime;
    struct tm * timeinfo;

    time (&rawtime);
    timeinfo = localtime (&rawtime);
    
    int now = timeinfo->tm_sec + timeinfo->tm_min*60 + timeinfo->tm_hour*60*60;
    return now == 0;
}

void print_usage(std::string program_name) {
    std::cout << program_name << " usage:" << std::endl <<
	    "\t--help   (-h): print this message." << std::endl <<
	    "\t--override (-O): override point picked parameters." <<
	    std::endl <<
	    "\t--reg_source (-s) <file_name>: Specify regular file where data comes from." <<
	    std::endl <<
	    "\t--dev_source (-D) <number>: Specify device number where data comes from." <<
	    std::endl <<
	    "\t--record (-r) <file_name>: Record video to filename." <<
	    std::endl <<
	    "\t--stream (-S) <device>: Streams video via device." <<
	    std::endl <<
	    "\t--sensor (-t) <device>: Specify device file which is a sensor." <<
	    std::endl <<
	    "\t--address (-a) <address>: address must be present and must be in " <<
	    "in this format: street-number. --address faria_lima-1200." <<
	    std::endl;
    return;
}

bool test_file(std::string file) {
    const char *c;;
    int fd;

    if(file.empty()) return true;

    c = file.c_str();
    if((fd = open(c, O_RDWR)) < 0) {
	    std::cout << "'" << file << "': "; std::cout.flush();
	    perror("The following error occurred");
	    return false;
    }
    else {
	    close(fd);
	    return true;
    }
}

void Print(const char *message, cv::Point position, cv::Mat &frame, unsigned int fontSize, unsigned int thickness) {
    cv::putText(frame, std::string(message),  position, CV_FONT_HERSHEY_PLAIN, fontSize, cv::Scalar(0, 0, 0), thickness, CV_AA);
}

void ProvidePip(cv::Mat &frame, cv::Mat &dst) {
    cv::Size pip1Size(std::min(400, frame.size().width), std::min(200, frame.size().height));
    cv::Rect pip1Rect(cv::Point(dst.size().width - pip1Size.width - 20, 20), pip1Size);
    cv::Mat pip1;
    cv::resize(frame, pip1, pip1Size);

    cv::rectangle(dst, pip1Rect, cv::Scalar(0, 0, 255), 5);
    pip1.copyTo(dst(pip1Rect));
}

void ProvideOsd(cv::Mat &frame, SensorData *sd, ObjectTracker &ot) {
    static cv::Mat logo = cv::imread("logo.jpg", CV_LOAD_IMAGE_COLOR);
    static cv::Mat cyclist = cv::imread("cyclist.jpg", CV_LOAD_IMAGE_COLOR);
    
    cv::Size sz = frame.size();
    cv::Point basePoint(0, sz.height - 80);
    int textVerticalSep = 20;
    int textTop = basePoint.y + 30;
    char mesgBuf[20];
    
    //Draw OSD
    cv::rectangle(frame, basePoint, cv::Point(sz.width, sz.height), cv::Scalar(255, 255, 255), CV_FILLED, 8,0);
    
    //Paste logo
    cv::Rect logoRect(cv::Point(10, (sz.height + basePoint.y - logo.size().height) / 2 ), logo.size());
    logo.copyTo(frame(logoRect));

    //Paste cyclist
    cv::Rect cyclistRect(cv::Point(frame.size().width-225, textTop - 25), cyclist.size());
    cyclist.copyTo(frame(cyclistRect));	

    //Put data    
    if(sd) {
	    //sprintf(mesgBuf, "CO: %s", sd.co.c_str());
	    //Print(mesgBuf, cv::Point(frame.size().width-170, textTop), frame);		

	    sprintf(mesgBuf, "Pressao: %s Pa", sd->pressure.c_str());
	    Print(mesgBuf, cv::Point(frame.size().width-160, textTop + textVerticalSep * 2), frame);

	    sprintf(mesgBuf, "Umidade: %s%%", sd->umidity.c_str());
	    Print(mesgBuf, cv::Point(frame.size().width-160, textTop + textVerticalSep), frame);	

	    sprintf(mesgBuf, "%s C", sd->temperature.c_str());
	    Print(mesgBuf, cv::Point(frame.size().width-125, textTop), frame, 2);

	    sprintf(mesgBuf, "%s", sd->temperature.c_str());		
	    sz = cv::getTextSize(mesgBuf, CV_FONT_HERSHEY_PLAIN, 2, 2, NULL);		
	    sprintf(mesgBuf, "o", sd->temperature.c_str());		
	    Print(mesgBuf, cv::Point(frame.size().width - 127 + sz.width, textTop - (sz.height) + 2), frame, 1);
    }

    sprintf(mesgBuf, "%d", ot.GetTotal());
    Print(mesgBuf, cv::Point(frame.size().width-300, textTop + 7), frame, 2);

    sprintf(mesgBuf, "Hoje");
    Print(mesgBuf, cv::Point(frame.size().width-275, textTop + textVerticalSep* 2), frame, 2);
		    
    time_t time_now = time(0);
    char buff[50];		
	    
    sprintf(buff, "Faria Lima");
    Print(buff, cv::Point(frame.size().width-410, textTop), frame, 1);		
	    
    strftime(buff, sizeof(buff), "%d/%m/%y", localtime(&time_now));
    Print(buff, cv::Point(frame.size().width-410, textTop + textVerticalSep), frame);		

    strftime(buff, sizeof(buff), "%X", localtime(&time_now));		
    Print(buff, cv::Point(frame.size().width-410, textTop + textVerticalSep * 2), frame);			
}

//Config class
Config::Config() {
    this->data = this->LoadData();		
}

Config::~Config() {
    this->PersistData(this->data);
}

unsigned int Config::GetLeftCounter() {
    return this->data.left_counter;	
}

void Config::SetLeftCounter(unsigned int counter) {
    this->data.left_counter = counter;
} 

unsigned int Config::GetRightCounter() {
    return this->data.right_counter;
}

void Config::SetRightCounter(unsigned int counter) {
    this->data.right_counter = counter;
} 

cv::Point Config::GetCounterPos(unsigned int index) {
    cv::Point pt;
    pt.x = this->data.x_counter[index];
    pt.y = this->data.y_counter[index];
    return pt;
}
void Config::SetCounterPos(unsigned int index, cv::Point pt) {
    this->data.x_counter[index] = pt.x;
    this->data.y_counter[index] = pt.y;    
}

cv::Point Config::GetPerspectivePos(unsigned int index) {
    cv::Point pt;
    pt.x = this->data.x[index];
    pt.y = this->data.y[index];
    return pt;    
}
void Config::SetPerspectivePos(unsigned int index, cv::Point pt) {
    this->data.x[index] = pt.x;
    this->data.y[index] = pt.y;     
}

cv::Point Config::GetCropPos(unsigned int index) {
    cv::Point pt;
    pt.x = this->data.x_crop[index];
    pt.y = this->data.y_crop[index];
    return pt;    
}
void Config::SetCropPos(unsigned int index, cv::Point pt) {
    this->data.x_crop[index] = pt.x;
    this->data.y_crop[index] = pt.y; 
}

cv::Point Config::GetInterestPos(unsigned int index) {
    cv::Point pt;
    pt.x = this->data.x_interest[index];
    pt.y = this->data.y_interest[index];
    return pt;    
}

void Config::SetInterestPos(unsigned int index, cv::Point pt) {
    this->data.x_interest[index] = pt.x;
    this->data.y_interest[index] = pt.y;     
}	

void Config::PersistData(configData &config) {
    int fd = open(this->configFile, O_WRONLY|O_CREAT, S_IRUSR | S_IWUSR);
    if (fd >= 0) {
	    write(fd, &config, sizeof(config));
	    close(fd);
    }			
}

configData Config::LoadData() {
    configData data;
    memset(&data, 0, sizeof(configData));
    int fd = open(this->configFile, O_RDONLY);
    if (fd >= 0) {
	    read(fd, &data, sizeof(data));
	    close(fd);
    }			
    return data;
}

