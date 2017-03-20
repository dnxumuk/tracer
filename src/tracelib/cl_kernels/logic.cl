// TODO: Add OpenCL kernel code here

// Currently the main task for the kernel is seen to generate ray for intersection

struct Ray { 
  float3 origin;
  float3 dir;
};

struct _ScreenParameters {
    uint frame_width;
    uint frame_height;

    float screen_coord_origin[3];
    float camera_origin[3];
    //
    float width_dir[3];
    float height_dir[3];

    // Real world coodinates per pixel
    float width_ratio;
    float height_ratio;
};

typedef struct _ScreenParameters ScreenParameters;



__kernel void logic(__global uchar * out, __global ScreenParameters *param) {
	// Need to be passed via params
	float x = get_global_id(0) / param->frame_width;
	float y = get_global_id(1) / param->frame_height;

	Ray ray;
	ray.origin = param->screen_coord_origin;
	float3 scr_pixel = screen_coord_origin - (param->width_dir * width_ratio * x) - (param->height_dir * height_ratio * y);
	ray.dir = normalize(scr_pixel - param->camera_origin);

	out[get_global_id(0)] = ray;
	// Also provide additional info. Let's use coloring as an angle between a ray and the axis from camera origin to world coordinates origin
	// The most little angle is the most staurated color thus 90 degree angle corresponds the black color
	float3 the_axis = normalize(param->camera_origin);
	float cosin = dot(the_axis, ray.dir);
	uint shift = 3*(get_global_id(1) * param->frame_width + get_global_id(0));
	out[shift] = cosin;
}