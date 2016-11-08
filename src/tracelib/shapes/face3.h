#pragma once
#include "ishape.h"

class Face3 :
  public IShape {
public:
  Face3();
  Face3(linearmath::vec3<float> points[3]);
  const linearmath::vec3<float>&  getNormal() const;
  virtual bool isIntersects(const Ray& ray) override;
  virtual linearmath::vec3<float> getColor(const Ray& ray,
                                           const linearmath::vec3<float>& point,
                                           const linearmath::vec3<float>& light) override;
  virtual ~Face3();
private:
  linearmath::vec3<float> normal_;
  std::pair<linearmath::vec3<float> , linearmath::vec3<float>> edges_[3];
  float distance_;
};


