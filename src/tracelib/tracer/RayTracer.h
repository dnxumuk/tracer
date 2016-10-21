#pragma once
#include <vector>
#include "..\shapes\ishape.h"
#include "..\shapes\sphere.h"
#include "..\linearmath\vector3d.h"
#include "ray.h"
#include "imagemap.h"
#include "..\tracer\scene.h"

class Scene;

class RayTracer
{
public:
	RayTracer();
	~RayTracer();
	std::string Run( Imagemap** imgBuffer);
	linearmath::vec3<float> Trace( const Ray& ray , const float distanse );
	void    setScene(Scene* scene);
	linearmath::vec3<float> getLight() { return _scene->getLight(0)->getCenter(); }
	void setLight(const linearmath::vec3<float>& newLight) { _scene->getLight(0)->setPosition(newLight, 0.5f ); };
	void refresh() { message = Run(&_imgBuffer); };
	std::string message;
	linearmath::vec3<float>  cameraPos;
	linearmath::vec3<float>  shader( const Ray& ray , RayIntersection& intPoint );
	bool findIntersections(const Ray& ray, RayIntersection& intersection);
private:
	Scene*  _scene;
	Imagemap *_imgBuffer;

};

