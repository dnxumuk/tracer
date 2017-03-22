#pragma once
#include <CL/cl.h>

struct cl_ray {
    float origin[3];
    float dir[3];
};


__declspec(align(32)) struct ScreenParameters {
    cl_uint frame_width;
    cl_uint frame_height;

    float screen_coord_origin[3];
    float camera_origin[3];
    //
    float width_dir[3];
    float height_dir[3];

    // Real world coodinates per pixel
    float width_ratio;
    float height_ratio;
};
