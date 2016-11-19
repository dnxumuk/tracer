#include "camera.h"
#include "ray.h"
#include <map>

Camera::Camera() {
}

Camera::~Camera() {
}

void Camera::getRay(size_t x, size_t y, Ray &ray) const {
  auto delta = getDxDY();
  auto screen_pixel = screen_position_ + screen_x_dir_*x*delta.first + screen_y_dir_*y*delta.second;
  auto dir = (screen_pixel - position_).getNormalized();
  ray.setOrigin(position_);
  ray.setDirection(dir);
}

void Camera::calculateScreen() {
  float cos_up = cosf(get_rotation(rotation_angle_));
  float sin_up = sinf(get_rotation(rotation_angle_));

  bool is_zero_y_axis_with_dir_cross = false;
  linearmath::vec3<float> y_axis(0.0, 0.0, 1.0);
  //if (is_zero_y_axis_with_dir_cross)
  if (y_axis.cross(direction_).lengthSqr() == 0.0)
    y_axis = {0.0f,1.0f,0.0f};
  linearmath::vec3<float> x_axis = y_axis * direction_;
  y_axis = direction_ * x_axis;
  
  upVector_ = x_axis*cos_up + y_axis*sin_up; // do i need this ? )
  // Calculate intersection of direction with screen plane
  auto intersection = position_ + direction_*camera_screen_distance_;
  // Claculate screen_origin
  auto intersection_scr_origin_dir = -(x_axis + y_axis).getNormalized();
  float screen_half_diagonal = camera_screen_distance_ * std::tanf(0.5f*fov_);
  screen_position_ = intersection + intersection_scr_origin_dir*screen_half_diagonal;
}


float to_degrees(float radians) {
  const float pi = 3.14159265359;
  return radians / pi * 180.0f;;
}

float to_radians(float degrees) {
  const float pi = 3.14159265359;
  return pi * degrees / 180.0f;
}

void Camera::setFOVDegrees(float degrees) {
  fov_ = to_radians(degrees);
}

void Camera::setFOVRadians(float radians) {
  fov_ = radians;
}

float Camera::getFOV(bool in_degrees /* = true */) {
  return (in_degrees) ? to_degrees(fov_) : fov_;
}

void Camera::setResolution(size_t width, size_t height) {
  width_ = width;
  height_ = height;
}

inline std::pair<size_t, size_t> Camera::getResolution() const {
  return std::make_pair(width_, height_);
}

inline linearmath::vec3<float> Camera::getPosition() const {
  return position_;
}

inline linearmath::vec3<float> Camera::getDirection() const {
  return direction_;
}

inline float Camera::getCameraScreenDistance() const {
  return camera_screen_distance_;
}

inline void Camera::setRotation(float angle) {
  rotation_angle_ = angle;
}

// Setters

inline void Camera::setCameraScreenDistance(float distance) {
  camera_screen_distance_ = distance;
}

inline void Camera::setPosition(const linearmath::vec3<float>& position, const linearmath::vec3<float>& direction) {
  position_ = position;
  direction_ = direction;
}

inline void Camera::setPosition(const linearmath::vec3<float>& position) {
  position_ = position;
}

inline void Camera::setLookAt(const linearmath::vec3<float>& lookAtPoint) {
  direction_ = (lookAtPoint - position_).getNormalized();
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
