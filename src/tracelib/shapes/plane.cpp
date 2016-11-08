#include "plane.h"
#include "..\tracer\rayintersection.h"
#include "../common/defaults.h"

static const float val = -50;

using namespace linearmath;

// The default plane is plane z=0
// the pane placed on X0Y plane
Plane::Plane() {
  _coef = {0.f, 0.f, 0.f};
  _distance = 0.f;
  _boundingBoxMin = {val, val, val};
  _boundingBoxMax = {-val, -val, -val};

  normal_ = {_coef.x,_coef.y,_coef.z};
  normal_.normalize();
}

Plane::Plane(const linearmath::vec3<float>& coef, float dParam) {
  _coef = coef;
  _distance = dParam;
  _boundingBoxMin = {val, val, val};
  _boundingBoxMax = {-val, -val, -val};

  normal_ = { -_coef.x,-_coef.y,-_coef.z };
  normal_.normalize();
}

Plane::~Plane() {
}

bool Plane::isCrossBoundingBox(const Ray & ray) {
  return false;
}

bool Plane::isIntersects(const Ray &ray) {
  // T = - (D-normal*origin)/ normal*direction
  // Intersection = origin + t*direction
  // Calculation of t
  float divider = _coef.dot(ray.getDirection());
  float dividen = _distance - _coef.dot(ray.getOrigin());
  if (divider != 0.0 && dividen != 0.0) {
    float coef_t = dividen / divider;
    RayIntersection &intersection = ray.getIntersection();
    if (coef_t >= 0 && coef_t < intersection.intsctDistance) {
      intersection.inssctColor = default_plane_color;
      intersection.intsctPosition = ray.getOrigin() + ray.getDirection()*coef_t;
      intersection.intsctDistance = coef_t;
      intersection.intsctNormal = getNormal();
      intersection.intsctSpecular = 0.f;
      return true;
    }
  }
  return false;
}

const linearmath::vec3<float>& Plane::getNormal() const {
  return normal_;
}

linearmath::vec3<float> Plane::getColor(const Ray &ray, 
    const linearmath::vec3<float>& point, 
    const linearmath::vec3<float>& light) {
  float coef = light.dot(getNormal()) / (light.length() * getNormal().length());
  if(coef <= 0.f)
    coef = 0.00001f;
  if (coef > 1.f)
    coef = 1.f;
  return linearmath::vec3<float>(0.f, 1.f, 0.f) * coef;
}
