#ifndef SENSORS__HPP
#define SENSORS__HPP

#include <iostream>
#include <thread>

typedef struct {
	int umidity;
	int temperature;
	int pressure;
	int co;
}SensorData;


std::thread *StartSensorsThread(const char *device, SensorData *sd);
#endif
