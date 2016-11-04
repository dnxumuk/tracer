#include "ray.h"
#include <memory.h>
using namespace linearmath;

Ray::Ray() {
}

Ray::Ray(const vec3<float>& origin, const vec3<float>& direction) :
  _origin(origin), 
  _direction(direction) {
  _direction.normalize();

  _inv_direction.x = (direction.x > 0.f) ? direction.x : 10e10f;
  _inv_direction.y = (direction.y > 0.f) ? direction.y : 10e10f;
  _inv_direction.z = (direction.z > 0.f) ? direction.z : 10e10f;
}

Ray::~Ray() {
}


// It might be thread unsafe, so must being used carefully 
const RayIntersection &Ray::getIntersectionConst() const {
  return _intersection;
}
