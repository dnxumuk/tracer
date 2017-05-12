#pragma once
#include "../linearmath/vector3d.h"

__declspec(align(32)) struct CLRay {
public:
  void SetOrigin(const linearmath::vec3<float> &point) {
    SetFloatArrayData(origin, point);
  }

  void SetDir(const linearmath::vec3<float> &point) {
    SetFloatArrayData(dir, point);
  }

private:

  void SetFloatArrayData(float *dst, const linearmath::vec3<float> &data) {
    if (dst) {
      dst[0] = data.x;
      dst[1] = data.y;
      dst[2] = data.z;
      dst[3] = 0.0;
    }
  }

  float origin[4];
  float dir[4];
  //
  float color[4];
  float dist;

};