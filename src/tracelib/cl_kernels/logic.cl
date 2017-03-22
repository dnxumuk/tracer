// TODO: Add OpenCL kernel code here

// Currently the main task for the kernel is seen to generate ray for intersection

struct _Ray { 
  float3 origin;
  float3 dir;
};

struct _ScreenParameters {
    uint frame_width;
    uint frame_height;

    float3 screen_coord_origin;
    float3 camera_origin;
    //
    float3 width_dir;
    float3 height_dir;

    // Real world coodinates per pixel
    float width_ratio;
    float height_ratio;
};

typedef struct _Ray Ray;
typedef struct _ScreenParameters ScreenParameters;



__kernel void logic(__global uchar * out, __global ScreenParameters *param) {
	// Need to be passed via params
	float x = get_global_id(0) / (float)param->frame_width;
	float y = get_global_id(1) / (float)param->frame_height;

	Ray ray;
	float3 scr_pixel = param->screen_coord_origin - (param->width_dir * param->width_ratio * x) - (param->height_dir * param->height_ratio * y);
	ray.dir = normalize(scr_pixel - param->camera_origin);

	// Also provide additional info. Let's use coloring as an angle between a ray and the axis from camera origin to world coordinates origin
	// The most little angle is the most staurated color thus 90 degree angle corresponds the black color

	//float3 scr_origin_norm = normalize(param->screen_coord_origin);
	float cosinnn = dot(ray.dir, param->camera_origin);
	uint shift = 3*(get_global_id(1) * param->frame_width + get_global_id(0));
	out[shift] = (uchar)(255.0*cosinnn);
	//out[shift+1] = (uchar)(255.0*y);
}