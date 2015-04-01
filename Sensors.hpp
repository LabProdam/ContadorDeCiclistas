#ifndef SENSORS__HPP
#define SENSORS__HPP

#include <iostream>
#include <thread>

typedef struct {
	std::string umidity;
	std::string temperature;
} SensorData;

std::thread *StartSensorsThread(const char *device, SensorData *sd);
#endif
