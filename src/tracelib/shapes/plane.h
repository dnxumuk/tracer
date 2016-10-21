#pragma once
#include "ishape.h"
#include "..\tracer\ray.h"

class Plane : public IShape {
public:
	Plane();
	Plane( linearmath::vec4<float>& coeff);
	~Plane();

	bool isCrossBoundingBox(const Ray& ray);
	virtual bool isIntersects(const Ray& ray, RayIntersection& intersection );
	virtual linearmath::vec3<float>&  getNormal(const linearmath::vec3<float>& point);
	virtual linearmath::vec3<float> getColor(const Ray& ray,
											 const linearmath::vec3<float>& point,
											 const linearmath::vec3<float>& light) ;
protected:
	linearmath::vec3<float> _boundingBoxMin;
	linearmath::vec3<float> _boundingBoxMax;
private:
	linearmath::vec4<float> _coef;
    // Normilized normal is stored
    linearmath::vec3<float> normal_;
	float _distance;
};