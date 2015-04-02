#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#include <regex>

#include "Sensors.hpp"
//TODO: mudar para metodo da classe Sensors
//separar funcoes dos sensores.
void RetrieveSensorData(const char *device, SensorData *sensorData) {
	struct termios options;
	int fd;

	//TODO colocar isso no construtor da classe.
	if(fd = open(device, O_RDWR) < 0) {
//		int errno2 = errno;
//		perror("The following error occurred");
//		std::cout << device << std::endl;
		exit(EXIT_FAILURE); //TODO nao ser tao drastico
	}

	tcgetattr(fd, &options);

	cfsetispeed(&options, B9600);
	cfsetospeed(&options, B9600);
	options.c_cflag |= (CLOCAL | CREAD);

	tcsetattr(fd, TCSANOW, &options);

	std::regex umidityRe("U:(\\d+)");
	std::regex temperatureRe("T2:(\\d+)");
	std::regex pressureRe("P:(\\d+)");
	std::regex coRe("MQ7:(\\d+)");

	char buff[80];
	int readBytes;

	while(1) {
		usleep(500000);
		memset(buff, 0, sizeof(buff));
		readBytes = read(fd, buff, sizeof(buff));
		if (readBytes) {		
			std::cmatch umidity;
			std::cmatch temperature;
			std::cmatch pressure;
			std::cmatch co;
			std::regex_search(buff, umidity, umidityRe);
			std::regex_search(buff, temperature, temperatureRe);
			std::regex_search(buff, pressure, pressureRe);
			std::regex_search(buff, co, coRe);
			if (umidity[1].length())
				sensorData->umidity = umidity[1];
			if (temperature[1].length())
				sensorData->temperature = temperature[1];
			if (pressure[1].length())
				sensorData->pressure = pressure[1];
			if (co[1].length())
				sensorData->co = co[1];
		}
	}
	close(fd); //TODO e isso no destrutor.
}

std::thread *StartSensorsThread(const char *device, SensorData *sd) {
	std::thread *sensors = new std::thread (RetrieveSensorData, device, sd);
	return sensors;
}
