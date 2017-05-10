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
};

typedef struct _Ray Ray;
typedef struct _ScreenParameters ScreenParameters;



__kernel void logic(__global uchar * out, __global ScreenParameters *param, __global float *debug) {
	// Need to be passed via params
	// Getting ray
	float4 scr_pixel = param->screen_coord_origin - 
	(param->width_dir  * param->width_ratio  * (float)get_global_id(0)) - 
	(param->height_dir * param->height_ratio * (float)get_global_id(1));

	float4 ray = normalize(scr_pixel - param->camera_origin);

	// Getting cental axis
	float4 axis_pt = param->screen_coord_origin - param->width_dir*param->width_ratio*param->frame_width/2.0 -
	param->height_dir*param->height_ratio*param->frame_height/2.0;

	float4 axis = normalize(axis_pt - param->camera_origin);

	float dtp = dot(ray,axis);
	// Also provide additional info. Let's use coloring as an angle between a ray and the axis from camera origin to world coordinates origin
	// The most little angle is the most staurated color thus 90 degree angle corresponds the black color

	//float3 scr_origin_norm = normalize(param->screen_coord_origin);
	//float cosinnn = dot(rrr1, axis1);

	uint shift = 3*(get_global_id(1) * param->frame_width + get_global_id(0));
	//out[shift] = (uchar)(255.0*get_global_id(0)/param->frame_width);
	out[shift] = (uchar)(255.0*dtp);
	out[shift+1] = (uchar)(255.0*dtp);
	out[shift+2] = (uchar)(255.0*dtp);

	//debug[shift] = dtp;
	//debug[shift+1] = 0.0;
	//debug[shift+2] = 0.0;


	//out[shift+1] = (uchar)(255.0*y);
}