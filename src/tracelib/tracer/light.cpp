#include "light.h"
#include "..\linearmath\vector3d.h"
#include "../common/defaults.h"

using namespace linearmath;

Light::Light() {
  _radius = 0.5;
  _color = {1.f , 1.f , 1.f};
}

Light::~Light() {
}

bool Light::isIntersects(const Ray & ray) {
  vec3<float> tmpVec = ray.getOrigin() - _center;
  float B = 2 * tmpVec.dot(ray.getDirection());
  float C = tmpVec.lengthSqr() - _radius * _radius;
  float discr = B*B - 4 * C;
  if (discr < 0)
    return false;
  
  // Calculating point's 
  float t2 = (-B + sqrt(discr)) * 0.5f;
  if (t2 < 0)
    return false;
  return true;
}

linearmath::vec3<float> Light::getNormal(const linearmath::vec3<float>& point) {
  return linearmath::vec3<float>();
}

linearmath::vec3<float> Light::getColor(const Ray & ray, const linearmath::vec3<float>& point, const linearmath::vec3<float>& light) {
  //vec3<float> m0m1 = ray.getOrigin() - _center;
  //vec3<float> cross = m0m1.cross( ray.getDirection() );
  //float result = cross.length() / ray.getDirection().length();
  //return linearmath::vec3<float>(1.f , 1.f, 1.f)*( (1.f - result)/_radius );
  return linearmath::vec3<float>(0.f, 1.f, 0.f);
}

linearmath::vec3<float> Light::getColorFong(const Ray & ray, const linearmath::vec3<float>& point, const linearmath::vec3<float>& light) {
  return getColor(ray, point, light);
}

inline bool Light::inBBox(const linearmath::vec3<float>& point) {
  return false;
}

void Light::setPosition(const linearmath::vec3<float>& pos, const float radius) {
  _center = pos;
  _radius = radius;
}
