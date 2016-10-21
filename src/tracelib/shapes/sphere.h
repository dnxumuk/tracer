#pragma once
#include "ishape.h"
#include "..\linearmath\vector3d.h"
class Sphere :
	public IShape
{
public:
	Sphere();
	Sphere( linearmath::vec3<float> center , float radius );
	virtual ~Sphere();
	void move  ( const linearmath::vec3<float>& vec );
	void resize( float newRadius);
	virtual bool isIntersects( const Ray& ray, RayIntersection& intersection);
	virtual linearmath::vec3<float>& getNormal( const linearmath::vec3<float>& point );
	virtual linearmath::vec3<float> getColor ( const Ray& ray, 
		                                       const linearmath::vec3<float>& point,
		                                       const linearmath::vec3<float>& light );
private:
	void invalidate();
	linearmath::vec3<float> _center;
	float               _radius;
};  

