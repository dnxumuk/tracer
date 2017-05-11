#pragma once
#include "../linearmath/vector3d.h"

__declspec(align(32)) struct CLRay {

  float getDistance() const {
    return distance_;
  }

  float setDistance(float value) {
    return distance_ = value;
  }

  void getColor(linearmath::vec3<float> &color) {
    GetFloatArrayData(color_,color);
  }

  void setColor(linearmath::vec3<float> &color) {
    SetFloatArrayData(color_, color);
  }

  void setRayOrientation(const linearmath::vec3<float> &origin, const linearmath::vec3<float> &dir) {
    SetFloatArrayData(origin_, origin);
    SetFloatArrayData(dir_, dir);
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
  void GetFloatArrayData(float *data, linearmath::vec3<float> &dst) {
    if (data) {
      dst.x = data[0];
      dst.y = data[1];
      dst.z = data[2];
    }
  }
  // Data memebers
  float distance_;
  float color_[4];
  float dir_[4];
  float origin_[4];
}