// TODO: Add OpenCL kernel code here

// Currently the main task for the kernel is seen to generate ray for intersection

struct Ray { 
  float3 origin;
  float3 dir;
};

__kernel void logic(__global uchar * out) {
	// Need to be passed via params
	float3 origin_ = { 0.0, 0.0, 10.0 };
	float3 scr_origin = { 0.0, 0.0, 0.0 };
	// 
	float scr_w = 1 / 10.0;
	float scr_h = 1 /10.0;

	float3 u_dir = { 1.0 ,0.0, 0.0};
	float3 v_dir = { 0.0 ,1.0 ,0.0};

	float pic_width = 500;
	float pic_height = 500;

	float x = get_global_id(0) / pic_width;
	float y = get_global_id(1) / pic_height;

	Ray ray;
	ray.origin = origin_;
	float3 scr_pixel = scr_origin - u_dir * scr_w * pic_width - v_dir * scr_h * pic_height;
	ray.dir = (scr_pixel - origin_).normilize;

	out[get_global_id(0)] = ray;
}