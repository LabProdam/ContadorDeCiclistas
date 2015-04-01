#ifndef UTILS__HPP
#define UTILS__HPP

#include<opencv2/opencv.hpp>

#include "Opt.hpp"
#include "ObjectTracker.hpp"
#ifdef SENSORS
#include "Sensors.hpp"
#endif

void print_usage(std::string program_name);

void Print(const char *message, cv::Point position, cv::Mat &frame, unsigned int fontSize = 1, unsigned int thickness = 1);

void ProvidePip(cv::Mat &frame, cv::Mat &dst);

#ifndef SENSORS
void ProvideOsd(cv::Mat &frame, ObjectTracker &ot);
#else
void ProvideOsd(cv::Mat &frame, SensorData &sd, ObjectTracker &ot);
#endif

#endif
