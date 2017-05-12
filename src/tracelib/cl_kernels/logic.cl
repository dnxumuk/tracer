// TODO: Add OpenCL kernel code here

// Currently the main task for the kernel is seen to generate ray for intersection

struct _CLRay {
  float4 origin;
  float4 dir;
  float4 color;
  float dist;
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
};

typedef struct _CLRay CLRay;
typedef struct _ScreenParameters ScreenParameters;



__kernel void logic(__global uchar *out, __global ScreenParameters *param, __global CLRay *rays) {
	// Need to be passed via params
	// Getting ray
	uint x_coord = get_global_id(0);
	uint y_coord = get_global_id(1);

	float4 scr_pixel = param->screen_coord_origin - 
	(param->width_dir  * param->width_ratio  * x_coord) - 
	(param->height_dir * param->height_ratio * y_coord);

	float4 ray1 = normalize(scr_pixel - param->camera_origin);

	// Getting cental axis
	float4 axis_pt = param->screen_coord_origin - 0.5f*(param->width_dir*param->width_ratio*param->frame_width +
													  param->height_dir*param->height_ratio*param->frame_height);

	float4 axis = normalize(axis_pt - param->camera_origin);

	float dtp = (uchar)255.0f*dot(ray1,axis);

	// Also provide additional info. Let's use coloring as an angle between a ray and the axis from camera origin to world coordinates origin
	// The most little angle is the most staurated color thus 90 degree angle corresponds the black color
	uint idx = y_coord * param->frame_width + x_coord;
	uint shift = 3*idx;

	out[shift]   = 0;
	out[shift+1] = 0;
	out[shift+2] = 255-dtp;
	
	// Forming of ray
	__global struct _CLRay *cur_ray = &rays[idx];

	cur_ray->origin = param->camera_origin;
	cur_ray->dir = ray1;

}