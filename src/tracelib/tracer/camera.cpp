#include "camera.h"
#include <map>

Camera::Camera() {
}

Camera::~Camera() {
}

float to_degrees(float radians) {
  const float pi = 3.14159265359;
  return 180.0f * pi / radians;
}

double focal_to_angle(double focal) {
  // Default focal's unit is 'mm'
  const float base_focal = 18.0f;
  const float min_focal = 0.1f;
  return (focal < min_focal) ? 180.0 : 2.0f * to_degrees(atan(base_focal / focal));
}

float get_fov(float camera_focal) {
  return focal_to_angle(camera_focal);
}

float get_camera_height(float width, float focal) {
  return 0.5f * width * tan(0.5*to_degrees(get_fov(focal));
}

float get_camera_fov(float width, float height, float focal) {
  return 2.0f * to_degrees(atan(0.5f*height / get_camera_height(width, focal)));
}

float get_rotation(float degrees) {
  return 3.1456f * (90.0f + degrees) / 180;
}

auto get_xy_axices(float rotation, const linearmath::vec3<float> &dir) {
  float cos_up = cosf(get_rotation(rotation));
  float sin_up = sinf(get_rotation(rotation));
  bool is_zero_y_axis_with_dir_cross = false;
  linearmath::vec3<float> y_axis(0.0, 0.0, 1.0);
  if (is_zero_y_axis_with_dir_cross)
    y_axis = {0.0f,1.0f,0.0f};
  linearmath::vec3<float> x_axis = y_axis * dir;
  y_axis = dir * x_axis;
  auto up_vec = x_axis*cos_up + y_axis*sin_up;
  return std::tuple<linearmath::vec3<float>, linearmath::vec3<float>, linearmath::vec3<float>>(x_axis,y_axis,up_vec);
}
