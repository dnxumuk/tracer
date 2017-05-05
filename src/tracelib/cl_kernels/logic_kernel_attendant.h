#pragma once
#include "../linearmath/vector3d.h"
#include <CL/cl.h>

struct cl_ray {
    float origin[3];
    float dir[3];
};


__declspec(align(32)) struct ScreenParameters {
public:
	void SetScreenOrigin(const linearmath::vec3<float> &point) {
		SetFloatArrayData(screen_coord_origin, point);
	}

	void SetCameraOrigin(const linearmath::vec3<float> &point) {
		SetFloatArrayData(camera_origin, point);
	}

	void SetWidthDir(const linearmath::vec3<float> &point) {
		SetFloatArrayData(width_dir, point);
	}

	void SetHeightDir(const linearmath::vec3<float> &point) {
		SetFloatArrayData(height_dir, point);
	}

    cl_uint frame_width;
    cl_uint frame_height;
    // Real world coodinates per pixel
    float width_ratio;
    float height_ratio;
private:

	void SetFloatArrayData(float *dst, const linearmath::vec3<float> &data) {
		if (dst) {
			dst[0] = data.x;
			dst[1] = data.y;
			dst[2] = data.z;
			dst[3] = 0.0;
		}
	}

	float screen_coord_origin[4];
	float camera_origin[4];
	//
	float width_dir[4];
	float height_dir[4];

};
