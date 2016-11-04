#pragma once
#include "../linearmath/vector3d.h"


struct RayIntersection {
  RayIntersection();
  float intsctDistance;
  linearmath::vec3<float> intsctPosition;
  linearmath::vec3<float> intsctNormal;
  linearmath::vec3<float> inssctColor;
  float intsctSpecular;
};