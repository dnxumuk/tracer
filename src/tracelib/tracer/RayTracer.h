#pragma once
#include <vector>
#include "..\shapes\ishape.h"
#include "..\shapes\sphere.h"
#include "..\linearmath\vector3d.h"
#include "ray.h"
#include "imagemap.h"
#include "..\tracer\scene.h"
#include <stack>

class RayTracer {
public:
  RayTracer();
  ~RayTracer();
  std::string Run();
  linearmath::vec3<float> Trace(const Ray &ray, float distanse);
  void setScene(Scene *scene);
  linearmath::vec3<float> getLight() { 
    return scene_->getLight(0)->getCenter();
  }
  void setLight(const linearmath::vec3<float> &newLight) {
    scene_->getLight(0)->setPosition(newLight, 0.5f); 
  }
  void refresh() { 
    message = Run();
  };
  linearmath::vec3<float> shader(const Ray &ray);
  bool findIntersections(const Ray &ray);
public:
  linearmath::vec3<float>  cameraPos;
  std::string message;
  double render_time_;
  Scene* scene_;
  Imagemap &imgBuffer_;
  std::stack<RayIntersection> intersection_point_;
};

