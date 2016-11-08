#pragma once
#include "ishape.h"
#include <vector>

class FaceN :
  public IShape {
public:
  FaceN(size_t n);
  FaceN(linearmath::vec3<float> *points, size_t n);
  const linearmath::vec3<float>&  getNormal() const;
  virtual bool isIntersects(const Ray& ray) override;
  virtual linearmath::vec3<float> getColor(const Ray& ray,
                                           const linearmath::vec3<float>& point,
                                           const linearmath::vec3<float>& light) override;
  virtual ~FaceN();
private:
  linearmath::vec3<float> normal_;
  std::vector<std::pair<linearmath::vec3<float>, linearmath::vec3<float>>> edges_;
  float distance_;
};