#pragma once
#include "../linearmath/vector3d.h"

class Camera {
public:
  Camera();
  ~Camera();
private:
  linearmath::vec3<float> position_;
  linearmath::vec3<float> direction_;
  linearmath::vec3<float> upVector_;
  float fov_;

};

