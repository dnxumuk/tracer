#pragma OPENCL EXTENSION cl_khr_byte_addressable_store : enable
__kernel void hello(__global uchar * out) {

	uint width = get_global_id(0);
	uint height = get_global_id(1);
	
	uint idx = height*1500 + 3*width;

	float2 pt = { width, height };
	float2 origin = { 200.0, 200.0 };

	float dst = distance(pt, origin);

	if ( dst < 200.0 )
		out[idx+2] = 255;
}