#include "RayTracer.h"
#include "scene.h"
#include "../linearmath/profiler.h"
#include "rayintersection.h"
#include <omp.h>
#pragma omp

using namespace linearmath;
using namespace std;

RayTracer::RayTracer() {
	_scene = nullptr;
	cameraPos = { 0.f , 0.f , 20.f };
}


inline float clamp(const float val, const float low, const float high)
{
	return (val < low) ? low : ((val > high) ? high : val);
}


RayTracer::~RayTracer() {
}

std::string RayTracer::Run( Imagemap** buffer) {
	if (buffer == nullptr || _scene == nullptr )
		return string();

	if  (*buffer == nullptr )
		*buffer = new Imagemap(250, 250);
	_imgBuffer = *buffer;
	auto img = *buffer;

	const size_t width  = img->width();
	const size_t height = img->height();

	std::string msgResults;
	std::string msgHeader;
	msgHeader = "Rendering image of " + std::to_string(img->width()*img->height()/ (float) 1'000'000) + " Mpx";
	Profiler prf(msgHeader, true);

	prf.start();
	const float cameraDist = 4.0f;
	const float  min = -1.0f * cameraDist;
	const float  max =  1.0f * cameraDist;
	float aspect = static_cast<float>( width ) / static_cast<float>( height );

	float dX = ( max - min ) / static_cast<float>( width ) * aspect;
	float dY = ( max - min ) / static_cast<float>( height );

	float sy = min;

	auto currentPix = img->getData();// +width*height * 4 - 4;
	float distance = 100.0f;

    Ray ray(cameraPos, vec3<float>());
  
	for ( size_t y(0); y < height ; ++y)
	{
		float sx = min;
		for (size_t x(0); x < width ; ++x)
		{
			const vec3<float> direction = vec3<float>(sx, sy, -10.f) -= cameraPos;
            ray.setDirection(direction);
			vec3<float> color = Trace( ray, distance) * 255;

			currentPix[0] = color.x;
			currentPix[1] = color.y;
			currentPix[2] = color.z;
			currentPix += 4;
			sx += dX;
		}
		sy += dY;
	}
	prf.finish();
	return prf.getResults();
}

vec3<float> RayTracer::Trace(const Ray & ray, const float distanse)
{
	vec3<float> result(1.f, 0.0f, 0.f);
	vec3<float> intersectPoint;
	size_t size = _scene->size();
	RayIntersection intersection;
	if (findIntersections( ray, intersection))
		result = shader( ray, intersection);
	return result;
	//for ( size_t i(0); i < size; ++i )
	//{
	//	auto light = _scene->getLight(0);
	//	if (  (*_scene)[i] &&
	//		  //(*_scene)[i]->isCrossBoundingBox( ray ) &&
	//		  (*_scene)[i]->isIntersects(ray, intersectPoint) )
	//	{
	//		return (*_scene)[i]->getColorFong(ray, intersectPoint, light->getCenter() );
	//	}
	//	if (_scene->getLight(0)->isIntersects(ray, intersectPoint))
	//	{
	//		return _scene->getLight(0)->getColor(ray, intersectPoint, light->getCenter() );
	//	}
	//}
/*/
	for (auto shape = _shapes->begin(); shape != _shapes->end(); ++shape)
		//	if ((*shape)->isCrossBoundingBox(ray))
		if ((*shape)->isIntersects(ray, intersectPoint))
		{
			float coef = ray.getDirection().dot(intersectPoint - light)	/ ( ray.getDirection().length() * (intersectPoint - light).length() );
			//coef *= 1.5;
			if (coef <= 0.f)
				coef = 0.00001f;
			if (coef > 1.f)
				coef = 1.f;

			assert(coef >= 0.f);
			assert(coef <= 1.f);

			vec3<float> color = { coef, coef, 0.f };
			return color;
		}
	return vec3<float>(0.f, 0.0f, 0.0f);
*/
}

void RayTracer::setScene(Scene * scene)
{
	if (scene)
		_scene = scene;
}

linearmath::vec3<float> RayTracer::shader(const Ray& ray, RayIntersection& intPoint)
{
	vec3<float> color = { 1.f , 0.f , 0.0f };
	vec3<float> diffuse = intPoint.inssctColor;
	for (size_t i(0); i < 1; i++)
	{
		auto light = _scene->getLight(i);
		vec3<float> dirToLight = (light->getCenter() - intPoint.intsctPosition).getNormalized();
		//float lightDistance = dirToLight.length();
		float NdotL = dirToLight.dot(intPoint.intsctNormal);

		Ray lightRay(intPoint.intsctPosition + (intPoint.intsctNormal * 0.001f), dirToLight);
		RayIntersection lightIntsct;
		if ( !findIntersections(lightRay, lightIntsct) )// Intersect
		{
			float lambert = clamp(NdotL, 0.f, 1.f);
			if (lambert > 0.f)
			{
				color = light->getColor()*diffuse*lambert;
				if (intPoint.intsctSpecular > 0.f)
				{
					auto V = ray.getDirection();
					auto R = V - intPoint.intsctNormal * 2 * V.dot(intPoint.intsctNormal);
					float VdotR = V.dot(R);
					if (VdotR > 0.0f)
					{
						float specularShiness = 32.f;
						float spec = pow(VdotR, specularShiness) * lambert * intPoint.intsctSpecular;
						// add specular component to ray color
						color += light->getColor() * spec;
					}
				}
			}
		}
	}
	color.x = clamp(color.x, 0.f, 1.f);
	color.y = clamp(color.y, 0.f, 1.f);
	color.z = clamp(color.z, 0.f, 1.f);
	return color;
}
bool RayTracer::findIntersections(const Ray & ray, RayIntersection & intersection)
{
	bool intersectionFound = false;
	for (size_t i(0); i < _scene->size() ; ++i)
	{
		if ((*_scene)[i]->isIntersects(ray, intersection))
			intersectionFound = true;
	}
	return intersectionFound;
}
//	foreach(light; scene.Lights)
//	{
//		vec3 dirToLight = light.Position - hitInfo.hitPosition;
//		float distToLight = dirToLight.length;
//		dirToLight.normalize();
//		float NdotL = dot(hitInfo.hitNormal, dirToLight);
//
//		Ray lightRay = new Ray(hitInfo.hitPosition + (hitInfo.hitNormal * 0.001f), dirToLight);
//		RayHitInfo lightHitInfo;
//		if (!this.FindIntersection(scene, lightRay, lightHitInfo, distToLight))
//		{
//			float lambert = clamp(NdotL, 0.0f, 1.0f);
//			if (lambert > 0.0f)
//			{
//				color.x += light.Color.x * matDiffuse.x * lambert;
//				color.y += light.Color.y * matDiffuse.y * lambert;
//				color.z += light.Color.z * matDiffuse.z * lambert;
//
//				// point light source: sample once for specular highlight
//				if (hitInfo.hitSpecular > 0.0f)
//				{
//					vec3 V = ray.Direction;
//					vec3 R = reflect(dirToLight, hitInfo.hitNormal).normalized;
//					float VdotR = dot(V, R);
//					if (VdotR > 0.0f)
//					{
//						float spec = pow(VdotR, 32.0f) * lambert * hitInfo.hitSpecular;
//						// add specular component to ray color
//						color.x += spec * light.Color.x;
//						color.y += spec * light.Color.y;
//						color.z += spec * light.Color.z;
//					}
//				}
//			}
//		}
//	}
//
//	color.x = clamp(color.x, 0.0f, 1.0f);
//	color.y = clamp(color.y, 0.0f, 1.0f);
//	color.z = clamp(color.z, 0.0f, 1.0f);
//
//	return color;
//}

