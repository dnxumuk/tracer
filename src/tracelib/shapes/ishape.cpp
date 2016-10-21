#include "ishape.h"
#include <math.h>

using namespace linearmath;

bool IShape::isCrossBoundingBox( const Ray& ray )
{
	vec3<float> dirfrac = ray.getInvDirection();
	auto origin = ray.getOriginPoint();
	vec3<float> t135 = (_boundingBoxMin - origin)*dirfrac;
	vec3<float> t246 = (_boundingBoxMax - origin)*dirfrac;
    # define max(a,b) ( (a) > (b) ? (a): (b) )
    # define min(a,b) ( (a) < (b) ? (a): (b) )
	float tmin = max(max(min(t135.x, t246.x), min(t135.y, t246.y)), min(t135.z, t246.z));
	float tmax = min(min(max(t135.x, t246.x), max(t135.y, t246.y)), max(t135.z, t246.z));
	#undef max
	#undef min
	if ( tmax < 0 || tmin > tmax )
		return false;

}


vec3<float>& IShape::getNormal( const vec3<float>& point )
{
	return vec3<float>(1.f,1.f,1.f);
}

bool IShape::inBBox(const vec3<float>& point)
{
	return (
		point.x >= _boundingBoxMin.x && point.x <= _boundingBoxMax.x &&
		point.y >= _boundingBoxMin.y && point.y <= _boundingBoxMax.y &&
		point.z >= _boundingBoxMin.z && point.z <= _boundingBoxMax.z);
	//	return true;
	//return false;
}

