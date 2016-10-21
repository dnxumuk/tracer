#include "plane.h"
#include "..\tracer\rayintersection.h"

static const float val = -50;

using namespace linearmath;
Plane::Plane() {
	_coef = { 0.f, 0.f, 0.f, 0.f };
	_distance = 0.f;
	_boundingBoxMin = { val, val, val };
	_boundingBoxMax = { -val, -val, -val };

    normal_ = {_coef.x,_coef.y,_coef.z};
    normal_.normalize();
}

Plane::Plane(linearmath::vec4<float>& coef) {
	_coef = coef;
	_distance = coef.w;
	_boundingBoxMin = { val, val, val };
	_boundingBoxMax = { -val, -val, -val };

    normal_ = { _coef.x,_coef.y,_coef.z };
    normal_.normalize();
}

Plane::~Plane() {
}

bool Plane::isCrossBoundingBox(const Ray & ray) {
	return false;
}

bool Plane::isIntersects(const Ray &ray, RayIntersection& intersection ) {
	auto normal = normal_;
	float d = normal.dot(ray.getDirection());

	vec3<float> p0;
	float pCoef = -_distance / (normal.x + normal.y + normal.z);
	p0 = normal*pCoef;

	//float domain = ray.getOriginPoint().dot( normal ) * -1.f;
	float domain = ((p0 - ray.getOriginPoint()).dot(normal)) / ( d );

	if (d != 0) {
		//domain /= d;
		//float dist = -( normal.dot( ray.getDirection().getNormalized() ) + _distance ) / d;
		if (domain < 0.f || domain > intersection.intsctDistance)
	    	return false;
		//auto iPoint = ray.getOriginPoint() + (ray.getDirection() * domain);
		//iPoint += normal * _distance;
		//float dist = -( d + distance ) / d;
		if ( domain > 0.f && domain < intersection.intsctDistance)
		{
			intersection.intsctPosition = ray.getOriginPoint() + ray.getInvDirection()*domain;
			intersection.inssctColor = vec3<float>(0.f, 1.f, 0.f);
			intersection.intsctDistance = domain;
			intersection.intsctNormal = this->getNormal(vec3<float>());
			intersection.intsctSpecular = 0.f;
			return true;
		}
	}
	return false;



	//// Calculating coefficients of the plane  (A,B,C,D)
	//vec4<float> planeCoeff = _coef;
	//vec3<float> planeABC;
	//planeABC.x = planeCoeff.x;
	//planeABC.y = planeCoeff.y;
	//planeABC.z = planeCoeff.z;
	//// This is the reight part of intersect point calculation equation
	//float dividen = -(planeABC.dot(ray.getOriginPoint()) + planeCoeff.w);
	//float divider =   planeABC.dot(ray.getDirection());

	//if (divider == 0.0f)
	//	return false;

	//// Getting t - pframetric coefficient t 
	//float t = dividen / divider;
	//// Getting intersection point coordinate and check it's inside bounding box face
	//float x = t * ray.getDirection().x + ray.getOriginPoint().x;
	//float y = t * ray.getDirection().y + ray.getOriginPoint().y;
	//float z = t * ray.getDirection().z + ray.getOriginPoint().z;

	//if (x < _boundingBoxMin.x || x > _boundingBoxMax.x)
	//	return false;
	//if (y < _boundingBoxMin.y || y > _boundingBoxMax.y)
	//	return false;
	//float z = t * ray.getDirection().z + ray.getOriginPoint().z;
	//if (z < _boundingBoxMin.z || z > _boundingBoxMax.z)
	//	return false;


	//intersection.intsctPosition = vec3<float>( x, y, z );
	//intersection.inssctColor = vec3<float>( 0.f, 1.f , 0.f );
	//intersection.intsctDistance = 1.f;
	//intersection.intsctNormal = this->getNormal( vec3<float>() );
	//intersection.intsctSpecular = 0.f;
	//return true;
}

linearmath::vec3<float>& Plane::getNormal(const linearmath::vec3<float>& point) {
	return linearmath::vec3<float>(_coef.x, _coef.y, _coef.z);
}

linearmath::vec3<float> Plane::getColor(const Ray &ray, 
    const linearmath::vec3<float>& point, 
    const linearmath::vec3<float>& light) {
//	float coef = ray.getDirection().dot( getNormal(point) ) /
//		(ray.getDirection().length() * ( getNormal(point) ).length());
	float coef = light.dot(getNormal(point))/ ( light.length() * getNormal(point).length() );
	if (coef <= 0.f)
		coef = 0.00001f;
	if (coef > 1.f)
		coef = 1.f;

	return linearmath::vec3<float>(0.f, 1.f, 0.f) * coef;
}
