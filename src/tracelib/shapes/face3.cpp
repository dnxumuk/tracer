#include "face3.h"
#include "../common/defaults.h"

Face3::Face3() {
}

Face3::Face3(linearmath::vec3<float> points[3]) {
  for (size_t i = 0; i < 3; ++i)
    edges_[i] = { points[i], points[(i+1)%3] - points[i] };

  normal_ = (edges_[0].second.cross(edges_[1].second)).getNormalized();
  distance_ = -normal_.dot(points[0]);
}

const linearmath::vec3<float>& Face3::getNormal() const {
  return normal_;
}

inline bool isOnRightSide(const linearmath::vec3<float>& point, 
                   const linearmath::vec3<float>& lineFrom,
                   const linearmath::vec3<float>& lineVec ) {
  return lineVec.crossSign(point - lineFrom) ;
}

bool Face3::isIntersects(const Ray & ray) {
  // T = - (D-normal*origin)/ normal*direction
  // Intersection = origin + t*direction
  //auto normal = normal_;
  // Calculation of t
  float divider = normal_.dot(ray.getDirection());
  float dividen = distance_ - normal_.dot(ray.getOrigin());
  if ( (divider <= 0.0f) != (dividen <= 0.0f) ) {
    return false;
  } else {
    float coef_t = dividen / divider;
    RayIntersection &intersection = ray.getIntersection();
    if (coef_t < intersection.intsctDistance) {

      linearmath::vec3<float> point_intersection = ray.getOrigin() + ray.getDirection()*coef_t;
      for (size_t i = 0; i < 3; ++i) {
        if (isOnRightSide(point_intersection, edges_[i].first, edges_[i].second))
          return false;
      }

      intersection.inssctColor = default_face_color;
      intersection.intsctPosition = point_intersection;
      intersection.intsctDistance = coef_t;
      intersection.intsctNormal = getNormal();
      intersection.intsctSpecular = 0.f;
      return true;
    }
  }
  return false;
}

linearmath::vec3<float> Face3::getColor(const Ray & ray, const linearmath::vec3<float>& point, const linearmath::vec3<float>& light ) {
  return linearmath::vec3<float>();
}

Face3::~Face3() {
}
