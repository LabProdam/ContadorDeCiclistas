#ifndef COORD_TRANSFORM_HPP
#define COORD_TRANSFORM_HPP

#include <opencv2/opencv.hpp>
#include "Camera.hpp"

class CoordTransform {
public:
	CoordTransform();
	~CoordTransform();
	
	//cam := calibrated camera.
	//P   := given a point P that resides in a frame, the method below will
	//       return its real Ry, i.e., its real height, assuming fixed Rz.
	static float GetRealRyFromImageiPoint(Camera& cam, cv::Point2f P);

	static float OpencvYCoord2PaperYCoord(Camera& cam, cv::Point2f P);
};

#endif
