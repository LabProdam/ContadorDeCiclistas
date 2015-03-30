#ifndef SENSORS__HPP
#define SENSORS__HPP

#include <iostream>
#include <thread>

typedef struct {
	std::string umidity;
	std::string temperature;
	std::string pressure;
	std::string co;
}SensorData;


std::thread *StartSensorsThread(const char *device, SensorData *sd);
#endif
