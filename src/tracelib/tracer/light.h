#pragma once
#include "../linearmath/vector3d.h"
#include "../shapes/ishape.h"

class Light : public IShape {
public:
  Light();
  ~Light();

  virtual bool isIntersects(const Ray& ray) override;
  virtual linearmath::vec3<float> getNormal(const linearmath::vec3<float>& point);
  virtual linearmath::vec3<float> getColor(const Ray& ray,
                                           const linearmath::vec3<float>& point,
                                           const linearmath::vec3<float>& light);
  virtual linearmath::vec3<float> getColorFong(const Ray& ray,
                                               const linearmath::vec3<float>& point,
                                               const linearmath::vec3<float>& light);

  bool inline inBBox(const linearmath::vec3<float>& point);
  void setPosition(const linearmath::vec3<float>& pos, const float center);

  void moveTo(const linearmath::vec3<float>& newPos) { _center = newPos; };

  linearmath::vec3<float> getCenter() { return _center; };
  linearmath::vec3<float> getColor() { return _color; };
  linearmath::vec3<float> getColor(const linearmath::vec3<float>& color) { _color = color; };

private:
  float _radius;
  linearmath::vec3<float> _center;
  linearmath::vec3<float> _color;
};