#include "RayTracer.h"
#include "scene.h"
#include "../linearmath/profiler.h"
#include "rayintersection.h"
#include <algorithm>
#include "../common/defaults.h"

#include "camera.h"

using namespace linearmath;

RayTracer::RayTracer() : imgBuffer_(*new Imagemap(500,400)) {
  scene_ = nullptr;
  //cameraPos = {0.0f , 0.0f , 100.0f};
  cameraPos = {0.0f , 0.0f , 100.0f};
}

RayTracer::~RayTracer() {}


inline float clamp(float val, float low, float high) {
  if (val>low && val<high)
    return val;
  if (val <= low)
    return low;
  else 
    return high;
}

std::string RayTracer::Run() {
  if (!scene_)
    return string();
  imgBuffer_.reset();

  // Create camera via setting up position and direction
  Camera camera;
  camera.setResolution(500,400);
  camera.setPosition({0.0f,0.0f,100.0f}, {0.0f,0.0f,-1.0f});
  camera.setRotation(0.0f);
  camera.setCameraToScreenDistance(4.0);
  camera.setFOVDegrees(60.0f);
  camera.setAspect(1.0f);

  const auto size   = camera.getResolution();
  const auto width  = size.first;
  const auto height = size.second;
  const float cameraDist = camera.getCameraToScreenDistance();
  const auto dxdy = camera.getDxDY();

  float dX = dxdy.first;
  float dY = dxdy.second;

  //
  std::string msgResults;
  std::string msgHeader = "Rendering image of " + 
    std::to_string(width * height / (float)1'000'000) + " Mpx";
  Profiler prf(msgHeader, true);

  prf.start();
  const float  min = -1.0f * cameraDist;
  //const float  max =  1.0f * cameraDist;
  //float aspect = width / static_cast<float>(height);

  //float dX = (max - min) / static_cast<float>(width) * aspect;
  //float dY = (max - min) / static_cast<float>(height);

  float sy = min;

  auto currentPix = imgBuffer_.getData();
  float distance = 100.0f;

  Ray ray(cameraPos, vec3<float>());
  
  linearmath::vec3<float> dx_dir = {};
  linearmath::vec3<float> dy_dir = {};

  
  for (size_t y(0); y < height; ++y) {
    float sx = min;
    for (size_t x(0); x < width; ++x) {
      camera.getRay(x,y,ray);
      //vec3<float> direction = vec3<float>(sx, sy, 90.f) - cameraPos;
      //ray.resetIntersection();
      //ray.setDirection(direction);
      vec3<float> color = Trace( ray, distance) * 255;

      currentPix[2] = static_cast<unsigned char>(color.x);
      currentPix[1] = static_cast<unsigned char>(color.y);
      currentPix[0] = static_cast<unsigned char>(color.z);
      currentPix += 3;
      sx += dX;
    }
    //sy += dY;
  }
  prf.finish();
  render_time_ = prf.getFPS();
  return prf.getResults();
}

vec3<float> RayTracer::Trace(const Ray &ray, float distanse) {
  if (findIntersections(ray)) {
    return shader(ray);
  } else {
    // The more set bits has default_stars_perion
    // the less stars number will be rendered
    if ((rand() & default_stars_period)) {
      return default_vacuum_color;
    } else {
      return default_star_color;
    }
  }
}

void RayTracer::setScene(Scene *scene) {
  if (scene)
    scene_ = scene;
}

linearmath::vec3<float> RayTracer::shader(const Ray& ray) {
  RayIntersection intersection_point = ray.getIntersectionConst();
  vec3<float> bg_color =// {1.0f,1.0f,0.0f};
  /*vec3<float> diffuse =*/ intersection_point.inssctColor;

  //ntPoint.inssctColor;
  for (size_t i=0; i < 1; ++i) {
    //auto light = scene_->getLight(i);
    auto light = scene_->getLight(i)->getColor();

    vec3<float> dirToLight = (-scene_->getLight(i)->getCenter() + intersection_point.intsctPosition).getNormalized();
    
    float NdotL = dirToLight.dot(intersection_point.intsctNormal);
    if (NdotL >= 0.0f)
      bg_color = bg_color*light*NdotL;
    else
      bg_color = {0.0f,0.0f,0.0f};

    // lightRay(intersection_point.intsctPosition + intersection_point.intsctNormal * 0.001f, dirToLight);
    //RayIntersection lightIntsct;
    /*
    if (findIntersections(lightRay)) {
      float lambert = clamp(NdotL, 0.f, 1.f);
      if (lambert > 0.f) {
        bg_color = light->getColor()*diffuse*lambert;

        if (intersection_point.intsctSpecular > 0.f) {
          vec3<float> V = ray.getDirection();
          vec3<float> R = V - intersection_point.intsctNormal * 2 * V.dot(intersection_point.intsctNormal);
          float VdotR = V.dot(R);
          if (VdotR > 0.0f) {
            float specularShiness = 32.f;
            float spec = pow(VdotR, specularShiness) * lambert * intersection_point.intsctSpecular;
            // add specular component to ray color
            bg_color += light->getColor() * spec;
          }

        }
      }*/
    //} else {
    //  bg_color = {0.,1.,1.};
    //}
  }

  bg_color.x = clamp(bg_color.x, 0.f, 1.f);
  bg_color.y = clamp(bg_color.y, 0.f, 1.f);
  bg_color.z = clamp(bg_color.z, 0.f, 1.f);
  return bg_color;
}

bool RayTracer::findIntersections(const Ray & ray) {
  bool intersectionFound = false;
  for (size_t i(0); i < scene_->size(); ++i) {
    if ((*scene_)[i]->isIntersects(ray)) {
      intersectionFound = true;
    }
  }
  return intersectionFound;
}
