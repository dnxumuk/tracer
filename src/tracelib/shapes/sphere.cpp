#include "sphere.h"

#include "..\linearmath\vector3d.h"
#include "..\linearmath\mat3x3.h"
#include "..\tracer\rayintersection.h"

using namespace linearmath;

Sphere::~Sphere() {
}

Sphere::Sphere() {
	this->_center = { 0.0f, 0.0f, 0.0f };
	this->_radius = 0.1f;
	invalidate();
}

Sphere::Sphere(linearmath::vec3<float> center, float radius) {
	_center = center;
	_radius = radius;
	invalidate();
}

void Sphere::move(const linearmath::vec3<float>& vec) {
	_center += vec;
	invalidate();
}

void Sphere::resize(float newRadius) {
	_radius = newRadius > 0 ? newRadius : _radius;
}

bool Sphere::isIntersects(const Ray &ray, RayIntersection &intersection ) {
	// calculating B,C coefficients for quadratic equation
	// A = 1; B = 2 * Dot(P0 - C, Dir); C = Length ^ 2(P0 - C) - R ^ 2
	vec3<float> tmpVec = ray.getOriginPoint() - _center;
	float B = 2 * tmpVec.dot(ray.getDirection());
	//
	float C = tmpVec.lengthSqr() - _radius * _radius;
	float discr = B*B - 4*C;
	if ( discr < 0)
		return false;
	// Calculating point's 
	float t1 = (-B - sqrt(discr)) * 0.5f;
	float t2 = (-B + sqrt(discr)) * 0.5f;
	float tValid = std::fmax( t1 , t2);
	if (tValid < 0)
		return false;
	if (intersection.intsctDistance <= tValid)
		return false;
	auto intPoint = ray.getOriginPoint() + ray.getDirection()*tValid;
	intersection.inssctColor = { 1.f , 1.f , 1.f };
	intersection.intsctDistance = tValid;
	intersection.intsctNormal = this->getNormal( intPoint );
	intersection.intsctPosition = intPoint;
	intersection.intsctSpecular = 1;

	return true;
}

vec3<float>& Sphere::getNormal(const vec3<float>& point) {
	return (point - _center).getNormalized();
}

linearmath::vec3<float> Sphere::getColor( const Ray& ray ,
	                                      const linearmath::vec3<float>& point,
	                                      const linearmath::vec3<float>& light ) {
/*
	float coef = ray.getDirection().dot( point - light) /
		(ray.getDirection().length() * ( point - light).length());
*/
	vec3<float> lightN = (light - point).getNormalized();

	float coef = lightN.dot( getNormal(point));
	
	if (coef <= 0.f)
		coef = 0.00001f;
	if (coef > 1.f)
		coef = 1.f;
	return linearmath::vec3<float>(1.f , 1.f, 1.f) * coef;
}

void Sphere::invalidate()
{
	_boundingBoxMin = { _center.x - _radius ,  _center.y - _radius ,  _center.z - _radius };
	_boundingBoxMax = { _center.x + _radius ,  _center.y + _radius ,  _center.z + _radius };
	_boundingMaxMin = _boundingBoxMax - _boundingBoxMin;
}