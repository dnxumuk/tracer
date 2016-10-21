#pragma once
#include <vector>

#include "..\shapes\ishape.h"
#include "light.h"

class Scene
{
public:
	Scene();
	Scene(size_t shapesCount);
	size_t size() const { return sceneSize; }
	void   addShape( IShape& shape);
	void addLight(Light & light);
	IShape* operator[](const size_t shapeNumber);
	Light* getLight( size_t lightNum );
	~Scene();
private:
	std::vector<IShape*> *_shapes;
	std::vector<Light*>  *_lights;
	size_t sceneSize;
};

