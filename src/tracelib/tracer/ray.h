#pragma once
#include "..\linearmath\vector3d.h"
#include "../tracer/rayintersection.h"
#include <memory>


class Ray {
public:
  Ray();
  Ray(const linearmath::vec3<float>& origin, 
      const linearmath::vec3<float>& direction);
  ~Ray();

  inline const linearmath::vec3<float>& getOrigin() const;
  inline const linearmath::vec3<float>& getDirection() const;
  inline const linearmath::vec3<float>& getInvDirection() const;

  inline void setDirection(const linearmath::vec3<float>& dir);
  inline void setOrigin(const linearmath::vec3<float>& origin);

  inline void setIntersection(const RayIntersection &source);
  inline void resetIntersection();
  RayIntersection& getIntersection() const ;
  const RayIntersection& getIntersectionConst() const;
private:
  linearmath::vec3<float> _origin;
  // Direction is normalized
  linearmath::vec3<float> _direction;
  linearmath::vec3<float> _inv_direction;
  mutable RayIntersection _intersection;
};

inline const linearmath::vec3<float>& Ray::getOrigin() const {
  return _origin;
}

inline const linearmath::vec3<float>& Ray::getDirection() const {
  return _direction;
}

inline const linearmath::vec3<float>& Ray::getInvDirection() const {
  return _inv_direction;
}

inline void Ray::setDirection(const linearmath::vec3<float>& dir) {
  _direction = dir.getNormalized();
  _inv_direction.x = 1.f / dir.x;
  _inv_direction.y = 1.f / dir.y;
  _inv_direction.z = 1.f / dir.z;
}

inline void Ray::setOrigin(const linearmath::vec3<float>& origin) {
  _origin = origin;
}

inline void Ray::setIntersection(const RayIntersection &source) {
  _intersection = source;
}

inline void Ray::resetIntersection() {
  memset(&_intersection, 0x0, sizeof(RayIntersection));
  _intersection.intsctDistance = 1e6;
}

inline RayIntersection &Ray::getIntersection() const {
  return _intersection;
}

