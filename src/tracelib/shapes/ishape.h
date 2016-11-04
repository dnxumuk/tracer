#pragma once
#include "..\linearmath\vector3d.h"
#include "..\linearmath\vector2d.h"
#include "..\linearmath\vector4d.h"
#include "..\tracer\ray.h"

struct RayIntersection;

class IShape {
public:
  bool isCrossBoundingBox(const Ray&ray);
  virtual bool isIntersects(const Ray&ray) = 0;
//  linearmath::vec3<float>& getNormal(const linearmath::vec3<float>& point) = 0;
  virtual linearmath::vec3<float> getColor(const Ray& ray,
                                           const linearmath::vec3<float>& point,
                                           const linearmath::vec3<float>& light) = 0;
  bool inline    inBBox(const linearmath::vec3<float>& point);
protected:
  linearmath::vec3<float> _boundingBoxMin;
  linearmath::vec3<float> _boundingBoxMax;
  linearmath::vec3<float> _boundingMaxMin;
	
};
