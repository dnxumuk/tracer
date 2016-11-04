#include "ishape.h"
#include <math.h>
#include  <algorithm>

using namespace linearmath;

bool IShape::isCrossBoundingBox(const Ray& ray) {
  vec3<float> dirfrac = ray.getInvDirection();
  auto origin = ray.getOrigin();
  vec3<float> t135 = (_boundingBoxMin - origin)*dirfrac;
  vec3<float> t246 = (_boundingBoxMax - origin)*dirfrac;
  float tmin = std::max(std::max(std::min(t135.x, t246.x), std::min(t135.y, t246.y)), std::min(t135.z, t246.z));
  float tmax = std::min(std::min(std::max(t135.x, t246.x), std::max(t135.y, t246.y)), std::max(t135.z, t246.z));
  if (tmax < 0 || tmin > tmax)
    return false;

}

//vec3<float>& IShape::getNormal( const vec3<float>& point ) {
//  return vec3<float>(0.f, 0.f, 0.f);
//}

bool IShape::inBBox(const vec3<float>& point) {
  return (
    point.x >= _boundingBoxMin.x && point.x <= _boundingBoxMax.x &&
    point.y >= _boundingBoxMin.y && point.y <= _boundingBoxMax.y &&
    point.z >= _boundingBoxMin.z && point.z <= _boundingBoxMax.z);
}

