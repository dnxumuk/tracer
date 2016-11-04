#include "scene.h"

Scene::Scene() {
  sceneSize = 0;
}

Scene::Scene(size_t shapesCount) {
  shapes_.reserve(shapesCount);
  sceneSize = shapesCount;
}


void Scene::addShape(IShape &shape) {
  shapes_.push_back(&shape);
  ++sceneSize;
}

void Scene::addLight( Light &light) {
  lights_.push_back(&light);
}


Light* Scene::getLight(size_t lightPos) {
  return lights_.at(lightPos);
}

IShape* Scene::operator[](size_t shapeNumber) {
   return (shapeNumber < sceneSize) ? shapes_[shapeNumber] : nullptr;
}

Scene::~Scene() {
}
