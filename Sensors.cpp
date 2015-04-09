#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#include "Sensors.hpp"
void RetrieveSensorData(const char *device, SensorData *sensorData) {
	struct termios options;
	int fd;

	if((fd = open(device, O_RDWR)) < 0) {
		perror("The following error occurred");
		std::cout << device << std::endl;
		exit(EXIT_FAILURE);
	}

	tcgetattr(fd, &options);

	cfsetispeed(&options, B9600);
	cfsetospeed(&options, B9600);
	options.c_cflag |= (CLOCAL | CREAD);

	tcsetattr(fd, TCSANOW, &options);
#define BUFFSIZE 200
	char buff[BUFFSIZE];
	int readBytes;

	while(1) {
		usleep(500000);
		memset(buff, 0, sizeof(buff));
		readBytes = read(fd, buff, sizeof(buff));
		buff[BUFFSIZE - 2] = '\n';
		buff[BUFFSIZE - 1] = '\0';
		printf("%s\n", buff);
		if (readBytes > 0) {
			int i = 0, colon_count = 0;
			bool stop = false;
			while((i < readBytes) && !stop) {
				if(buff[i++] == ':') {
					colon_count++;
//We are assuming that string provided by sensor is formated as follow:
//U:xx.xx%    T1:xx.xxC    T2:xx.xxC     P:xPa     Q7:xx
					switch(colon_count) {
						case 1:
							sscanf(buff + i, "%d", &sensorData->umidity);
							break;
						case 2:
							break;
						case 3:
							sscanf(buff + i, "%d", &sensorData->temperature);
							break;
						case 4:
							sscanf(buff + i, "%d", &sensorData->pressure);
							break;
						case 5:
							sscanf(buff + i, "%d", &sensorData->co);
							stop = true;
							break;
						default:
							std::cout << "error fetching sensor data\n" <<
								std::endl;
							break;
					}
				}
			}
		}
	}
	close(fd);
}

std::thread *StartSensorsThread(const char *device, SensorData *sd) {
	std::thread *sensors = new std::thread (RetrieveSensorData, device, sd);
	return sensors;
}
