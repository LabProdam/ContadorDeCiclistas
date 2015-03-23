#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#include <regex>

#include "Sensors.hpp"

void RetrieveSensorData(const char *device, SensorData *sensorData) {
	struct termios options;
	int fd;

	fd = open(device, O_RDWR);
	if (fd > 0) {
		tcgetattr(fd, &options);

		cfsetispeed(&options, B9600);
		cfsetospeed(&options, B9600);
		options.c_cflag |= (CLOCAL | CREAD);

		tcsetattr(fd, TCSANOW, &options);

		std::regex umidityRe("U:(\\d+)");
		std::regex temperatureRe("T:(\\d+)");

		char buff[80];
		int readBytes;
		while(1) {
			usleep(500000);
			memset(buff, 0, sizeof(buff));
			readBytes = read(fd, buff, sizeof(buff));
			if (readBytes) {		
				std::cmatch umidity;
				std::cmatch temperature;
				std::regex_search(buff, umidity, umidityRe);
				std::regex_search(buff, temperature, temperatureRe);
				if (umidity[1].length())
					sensorData->umidity = umidity[1];
				if (temperature[1].length())
				sensorData->temperature = temperature[1];
			}
		}

		close(fd);
	}
}

std::thread *StartSensorsThread(const char *device, SensorData *sd) {
	std::thread *sensors = new std::thread (RetrieveSensorData, device, sd);
	return sensors;
}
