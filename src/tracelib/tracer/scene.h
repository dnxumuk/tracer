#pragma once
#include <vector>

#include "..\shapes\ishape.h"
#include "light.h"

class Scene {
public:
  Scene();
  Scene(size_t shapesCount);
  size_t size() const {
    return sceneSize;
  }
  void addShape(IShape& shape);
  void addLight(Light & light);
  IShape* operator[](size_t shapeNumber);
  Light* getLight(size_t lightNum);
  ~Scene();
private:
  std::vector<IShape*> shapes_;
  std::vector<Light*>  lights_;
  size_t sceneSize;
};

