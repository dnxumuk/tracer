// TODO: Add OpenCL kernel code here

// Currently the main task for the kernel is seen to generate ray for intersection

struct _Ray { 
  float3 origin;
  float3 dir;
};

struct _ScreenParameters {
    uint frame_width;
    uint frame_height;

    // Real world coodinates per pixel
    float width_ratio;
    float height_ratio;

    float4 screen_coord_origin;
    float4 camera_origin;
    //
    float4 width_dir;
    float4 height_dir;
	//
    float4 axis_dir;
};

typedef struct _Ray Ray;
typedef struct _ScreenParameters ScreenParameters;



__kernel void logic(__global uchar * out, __global ScreenParameters *param) {
	// Need to be passed via params
	// Getting ray
	uint x = get_global_id(0);
	uint y = get_global_id(1);

	float4 scr_pixel = param->screen_coord_origin - (param->width_dir * param->width_ratio * x) - (param->height_dir * param->height_ratio * y);

	float4 ray = normalize(scr_pixel - param->camera_origin);
	float dtp = (uchar)255.0f*dot(ray,param->axis_dir);

	// Also provide additional info. Let's use coloring as an angle between a ray and the axis from camera origin to world coordinates origin
	// The most little angle is the most staurated color thus 90 degree angle corresponds the black color

	uint shift = 3*(y * param->frame_width + x);
	out[shift]   = dtp;
	out[shift+1] = dtp;
	out[shift+2] = dtp;
}