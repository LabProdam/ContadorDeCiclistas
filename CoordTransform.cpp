#include "CoordTransform.hpp"

CoordTransform::CoordTransform() {}
CoordTransform::~CoordTransform() {}

float CoordTransform::GetRealRyFromImageiPoint(Camera& cam, cv::Point2f P) {
	float Ry;
	float fd  = cam.GetFocalDistance();
	float cos = cam.GetCosTheta();
	float sin = cam.GetSinTheta();

	if(cam.GetRz() < 10e-5) {
		std::cout << "Error: Cam.Rz not set." << std::endl;
		exit(EXIT_FAILURE);
	}

	P.y = OpencvYCoord2PaperYCoord(cam, P);

	Ry  =  -cam.GetRz();
	Ry *=  (fd * sin - P.y * cos);
	Ry /=  (fd * cos + P.y * sin);
	Ry += cam.GetHeight();

	return Ry;
}
	
float CoordTransform::OpencvYCoord2PaperYCoord(Camera& cam, cv::Point2f P) {
	return -(P.y - cam.GetFrameRows() / 2.);
}
