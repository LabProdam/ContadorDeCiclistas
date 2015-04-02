#ifndef CAMERA__H
#define CAMERA__H

#include<opencv2/opencv.hpp>

class Camera {
private:
	float frameRows;
	float frameCols;
	float theta;
	float cosTheta;
	float sinTheta;
	float focalDistance;
	float height;
	float Rz; //distance from its projection on the ground(xz plane) to
			  //ciclyst
public:
	Camera();
	Camera(std::string confFileName);
	~Camera();

	void SetFrameRows(int rows);
	int  GetFrameRows(void);
	void SetFrameCols(int cols);
	int  GetFrameCols(void);

	//d  := known distance from camera to object.
	//h  := height of the real object.
	//P1 := From image, bottom point of the object.
	//P2 := From image, top point of the object.
	void SetFocalDistance(float d, float h, cv::Point2f P1, cv::Point2f P2);
	float GetFocalDistance(void);

	//lr := lengh of the real object.
	//P1 := From image, bottom point of the object.
	//P2 := From image, top point of the object.
	void SetHeight(float lr, cv::Point2f P1, cv::Point2f P2);
	float GetHeight(void);

	//f  := a captured frame.
	//l  := norm(Rz2 - Rz1).
	//P1 := From image, bottom point of the object.
	//P2 := From image, top point of the object.
	//h  := height of known object.
	void SetTheta(float l, cv::Point2f P1, cv::Point2f P2, float h);
	float GetTheta(void);
	float GetCosTheta(void);
	float GetSinTheta(void);

	//P := ?
	//h := real object height.
	void SetRz(cv::Point2f P, float h);
	float GetRz(void);

	void SaveConf(std::string confFileName);
};
#endif
