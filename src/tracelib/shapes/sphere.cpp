#include "sphere.h"

#include "..\linearmath\vector3d.h"
#include "..\linearmath\mat3x3.h"
#include "..\tracer\rayintersection.h"
#include "../common/defaults.h"
using namespace linearmath;

Sphere::~Sphere() {
}

Sphere::Sphere() {
  _center = {0.0f, 0.0f, 0.0f};
  _radius = 0.1f;
  invalidate();
}

Sphere::Sphere(const linearmath::vec3<float> &center, float radius) {
  _center = center;
  _radius = radius;
  invalidate();
}

void Sphere::move(const linearmath::vec3<float>& vec) {
  _center += vec;
  invalidate();
}

void Sphere::resize(float newRadius) {
  _radius = newRadius > 0 ? newRadius : _radius;
}

bool Sphere::isIntersects(const Ray &ray) {
  // calculating B,C coefficients for quadratic equation
  // A = 1; 
  // B = 2 * Dot(P0 - C, Dir); 
  // C = Length ^ 2(P0 - C) - R ^ 2

  vec3<float> tmpVec = ray.getOrigin() - _center;

  float B = 2*tmpVec.dot(ray.getDirection());
  float C = 4*(tmpVec.lengthSqr() - _r2);
  const float discr = B*B - C;

  if (discr >= 0) {
    const float sqr_root = sqrt(discr);

    float t1 = 0.5f*(-B - sqr_root);
    float t2 = 0.5f*(-B + sqr_root);
    float tValid = std::fmax(t1, t2);
  
    if (tValid < 0)
      return false;

    if (ray.getIntersectionConst().intsctDistance <= tValid)
      return false;

    RayIntersection &intersection = ray.getIntersection();
    intersection.inssctColor = default_sphere_color;

    intersection.intsctPosition = ray.getOrigin() + ray.getDirection()*tValid;
    intersection.intsctNormal = getNormal(intersection.intsctPosition);
    intersection.intsctDistance = tValid;
    intersection.intsctSpecular = 1;
    return true;
  }
  return false;
}

const vec3<float> Sphere::getNormal(const vec3<float>& point) {
  return (point - _center).getNormalized();
}

linearmath::vec3<float> Sphere::getColor(const Ray& ray ,
                                         const linearmath::vec3<float>& point,
                                         const linearmath::vec3<float>& light) {
  vec3<float> lightN = (light - point).getNormalized();

  float coef = lightN.dot(getNormal(point));
  if (coef <= 0.f)
    coef = 0.00001f;
  if (coef > 1.f)
    coef = 1.f;
  return linearmath::vec3<float>(1.f, 1.f, 1.f) * coef;
}

void Sphere::invalidate() {
  _r2 = _radius*_radius;
}