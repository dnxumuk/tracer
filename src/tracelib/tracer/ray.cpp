#include "ray.h"

using namespace linearmath;

Ray::Ray() {
}

Ray::Ray(const vec3<float>& origin, const vec3<float>& direction) :
	_origin(origin), _direction(direction) {
    _direction = direction.getNormalized();
	_invDirection = { 1.f / direction.x, 1.f / direction.y, 1.f / direction.z, };
}

Ray::~Ray() {
}

linearmath::vec3<float> Ray::getOriginPoint() const {
	return _origin;
}

linearmath::vec3<float> Ray::getDirection() const {
	return _direction;
}

linearmath::vec3<float> Ray::getInvDirection() const {
	return _invDirection;
}

void Ray::setDirection(const linearmath::vec3<float>& dir)
{
	_direction = dir.getNormalized();
	_invDirection.x = 1.f / dir.x;
	_invDirection.y = 1.f / dir.y;
	_invDirection.z = 1.f / dir.z;
}
