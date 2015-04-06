#ifndef UTILS__HPP
#define UTILS__HPP

#include<opencv2/opencv.hpp>

#include "Opt.hpp"
#include "ObjectTracker.hpp"
//#ifdef SENSORS
#include "Sensors.hpp"
//#endif

#include<time.h>

bool IsMidnight();

void print_usage(std::string program_name);
bool test_file(std::string file);

void Print(const char *message, cv::Point position, cv::Mat &frame, unsigned int fontSize = 1, unsigned int thickness = 1);

void ProvidePip(cv::Mat &frame, cv::Mat &dst);

void ProvideOsd(cv::Mat &frame, SensorData *sd, ObjectTracker &ot);

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
typedef struct {
		unsigned int left_counter;
		unsigned int right_counter;
		
		int x_counter[2];
		int y_counter[2];

		int x[4];
		int y[4];

		int x_crop[2];
		int y_crop[2];

		int x_interest[2];
		int y_interest[2];
		
}configData;

class Config {
private:
	const char *configFile = ".data";
	configData data;
public:
	Config();
	~Config();
	unsigned int GetLeftCounter();
	void SetLeftCounter(unsigned int counter);
	unsigned int GetRightCounter();
	void SetRightCounter(unsigned int counter);
	
	cv::Point GetCounterPos(unsigned int index);
	void SetCounterPos(unsigned int index, cv::Point pt);
	
	cv::Point GetPerspectivePos(unsigned int index);
	void SetPerspectivePos(unsigned int index, cv::Point pt);
	
	cv::Point GetCropPos(unsigned int index);
	void SetCropPos(unsigned int index, cv::Point pt);
	
	cv::Point GetInterestPos(unsigned int index);
	void SetInterestPos(unsigned int index, cv::Point pt);	
	
protected:
	void PersistData(configData &config);	
	configData LoadData();
	
};

#endif
