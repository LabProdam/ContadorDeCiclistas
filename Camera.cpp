#include <math.h>
#include <fstream>
#include <iostream>
#include "Camera.hpp"
#include "CoordTransform.hpp"

#define PI (4 * atan(tan(1.)))

Camera::Camera() {}
Camera::Camera(std::string confFileName) {
	std::ifstream confFile;
	confFile.open(confFileName);
	if(!confFile.is_open()) {
		std::cout << "Could not load camera configuration file: " <<
			confFileName << std::endl;
		return;
	}
	confFile >> this->frameRows;
	confFile >> this->frameCols;
	confFile >> this->theta;
	confFile >> this->cosTheta;
	confFile >> this->sinTheta;
	confFile >> this->focalDistance;
	confFile >> this->height;
	confFile >> this->Rz;
	confFile.close();
}
Camera::~Camera() {}


void Camera::SetFrameRows(int rows) {
	this->frameRows = rows;
}
int Camera::GetFrameRows(void) {
	return this->frameRows;
}
void Camera::SetFrameCols(int cols) {
	this->frameCols = cols;
}
int Camera::GetFrameCols(void) {
	return this->frameCols;
}

void Camera::SetFocalDistance(float d, float h, cv::Point2f P1,
							  cv::Point2f P2) {
	this->focalDistance = d * norm(P1 - P2) / h; //point - point is a vector
}
float Camera::GetFocalDistance(void) {
	return this->focalDistance;
}

void Camera::SetHeight(float lr, cv::Point2f P1, cv::Point2f P2) {
	this->height = this->focalDistance * lr / norm(P1 - P2);
}
float Camera::GetHeight(void) {
	return this->height;
}


//it does not matter if SetTheta receives P1 and P2 crossing
//	y = cam.frame.rows / 2
void Camera::SetTheta(float l, cv::Point2f P1, cv::Point2f P2, float h) {
	double k, error, error2, theta, theta_increment;
	double fd = this->focalDistance;

	int i;

	P1.y = CoordTransform::OpencvYCoord2PaperYCoord(*this, P1);
	P2.y = CoordTransform::OpencvYCoord2PaperYCoord(*this, P2);

	theta = 0.;
	theta_increment = PI / 180.;
	error2 = 0.;
	k = fabs(l / (h - this->height));
	for(i = 0, error = DBL_MAX;
			(error > 10e-10) && (fabs(theta) <= PI / 2.) &&	(i < 1000);
			i++) {
		error2  = (fd*cos(theta) + P1.y*sin(theta)) /
				  (fd*sin(theta) - P1.y*cos(theta));
		error2 -= (fd*cos(theta) + P2.y*sin(theta)) /
				  (fd*sin(theta) - P2.y*cos(theta));
		error2  = fabs(error2);
		error2  = fabs(k - error2);
//		std::cout << "theta = " << theta*180./PI << "\ti = " << i <<
//			"      error = " << error << "  error2 =" << error2 << std::endl;
		if(error2 > error) {
			theta_increment *= -.5;
//			std::cout << "\t" << theta_increment*180/PI <<"°"<< std::endl;
		}
		theta += theta_increment;
		error = error2;
	}
	if(theta < 0.) {
		std::cout << "theta = " << theta << " < 0." << std::endl <<
			"Aborting." << std::endl;
		exit(EXIT_FAILURE);
	}
	this->theta = theta;
	this->cosTheta = cos(theta);
	this->sinTheta = sin(theta);
}

float Camera::GetTheta(void) {
	return this->theta;
}
float Camera::GetCosTheta(void) {
	return this->cosTheta;
}
float Camera::GetSinTheta(void) {
	return this->sinTheta;
}

void Camera::SetRz(cv::Point2f P, float h) {
	float Rz;

	P.y = CoordTransform::OpencvYCoord2PaperYCoord(*this, P);

	Rz  = (this->focalDistance * this->cosTheta + P.y * this->sinTheta);
	Rz /= (this->focalDistance * this->sinTheta - P.y * this->cosTheta);
	Rz *= (this->height - h);

	this->Rz = Rz;
}
float Camera::GetRz(void) {
	return this->Rz;
}

void Camera::SaveConf(std::string confFileName) {
	std::ofstream confFile;
	confFile.open(confFileName, std::ios::trunc);
	if(!confFile.is_open()) {
		std::cout << "Could not save camera configuration file: " <<
			confFileName<< std::endl;
		return;
	}
	confFile <<
		this->frameRows << std::endl <<
		this->frameCols << std::endl <<
		this->theta << std::endl <<
		this->cosTheta << std::endl <<
		this->sinTheta << std::endl <<
		this->focalDistance << std::endl <<
		this->height << std::endl <<
		this->Rz << std::endl;
	confFile.close();
}
