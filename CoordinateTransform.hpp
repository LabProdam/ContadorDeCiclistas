#include <math.h>

/* Considers Y translation and X rotation
 */
class CoordinateTransform {
private:
	float cos_theta_rad;
	float sin_theta_rad;
	float focal_distance_m;
	float elevation_m;
public:
	CoordinateTransform(float theta_rad, 
			    float focal_distance_m,
			    float elevation_m) {
		this->sin_theta_rad = sin(theta_rad);
		this->cos_theta_rad = cos(theta_rad);
		this->focal_distance_m = focal_distance_m;
		this->elevation_m = elevation_m;	
	}

	float YPixelToM(float Py, float Rz) {
			
	}
};
