#include "scene.h"

Scene::Scene() {
	this->_shapes = new std::vector<IShape*>();
	this->_lights = new std::vector<Light*>();
	sceneSize = 0;
}

Scene::Scene(size_t shapesCount) {
	this->_shapes = new std::vector<IShape*>(shapesCount, nullptr);
	this->_lights = new std::vector<Light*>();
	sceneSize = 0;
}


void Scene::addShape( IShape & shape ) {
	_shapes->push_back( &shape );
	sceneSize++;
}

void Scene::addLight( Light & light) {
	_lights->push_back(&light);
}


Light* Scene::getLight(size_t lightPos) {
	return _lights->at(lightPos);
}

IShape * Scene::operator[](const size_t shapeNumber) {
	if ( shapeNumber > sceneSize )
		return nullptr;
	return(*this->_shapes)[shapeNumber];

}

Scene::~Scene() {
	delete _shapes;
	delete _lights;
}
