#pragma once
#include "ishape.h"
#include "..\tracer\ray.h"

class Plane : public IShape {
public:
  Plane();
  Plane(const linearmath::vec3<float>& coeff, float dParam);
  ~Plane();

  bool isCrossBoundingBox(const Ray& ray);
  virtual bool isIntersects(const Ray& ray) override;
  const linearmath::vec3<float>&  getNormal() const ;
  virtual linearmath::vec3<float> getColor(const Ray& ray,
      const linearmath::vec3<float>& point,
      const linearmath::vec3<float>& light);
protected:
  linearmath::vec3<float> _boundingBoxMin;
  linearmath::vec3<float> _boundingBoxMax;
private:
  linearmath::vec3<float> _coef;
  // Normilized normal is stored
  linearmath::vec3<float> normal_;
  float _distance;
  float dividen_ = -666.0;
};