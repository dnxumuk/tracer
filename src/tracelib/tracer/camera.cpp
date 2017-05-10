#include "camera.h"
#include "ray.h"
#include <map>

const float pi = 3.14159265359f;

Camera::Camera() {
}

Camera::~Camera() {
}

// Utility functons

float radians(float degrees) {
  return pi * degrees / 180.0f;
}

float get_rotation(float degrees) {
  return radians(90.0f + degrees);
}

float degrees(float radians) {
  return radians / pi * 180.0f;;
}

float focal_to_angle(float focal) {
  // Default focal's unit is 'mm'
  const float base_focal = 18.0f;
  const float min_focal = 0.1f;
  return (focal < min_focal) ? 180.0f : 2.0f * degrees(atan(base_focal / focal));
}

// Set and get field of view values
void Camera::setFOVDegrees(float degrees) {
  fov_ = radians(degrees);
}

void Camera::setFOVRadians(float radians) {
  fov_ = radians;
}

float Camera::getFOV(bool in_degrees /* = true */) {
  return (in_degrees) ? degrees(fov_) : fov_;
}

// Resolution

void Camera::setResolution(size_t width, size_t height) {
  width_ = width;
  height_ = height;
}

std::pair<size_t, size_t> Camera::getResolution() const {
  return std::make_pair(width_, height_);
}


// Camera positioning methods

linearmath::vec3<float> Camera::getPosition() const {
  return position_;
}

linearmath::vec3<float> Camera::getDirection() const {
  return direction_;
}

float Camera::getCameraToScreenDistance() const {
  return camera_screen_distance_;
}

void Camera::setRotation(float angle) {
  rotation_angle_ = angle;
  calculateScreen();
}

void Camera::setCameraToScreenDistance(float distance) {
  camera_screen_distance_ = distance;
  calculateScreen();
}

void Camera::setPosition(const linearmath::vec3<float>& position, const linearmath::vec3<float>& direction) {
  position_ = position;
  direction_ = direction;
  calculateScreen();
}

void Camera::setPosition(const linearmath::vec3<float>& position) {
  position_ = position;
}

void Camera::setLookAt(const linearmath::vec3<float>& lookAtPoint) {
  direction_ = (lookAtPoint - position_).getNormalized();
  calculateScreen();
}

//

void Camera::getRay(size_t x, size_t y, Ray &ray) const {
  auto delta = getDxDY();
  auto pixel_on_scr = scr_origin_ + -scr_u_dir_*static_cast<float>(x)*delta.first - scr_v_dir_*static_cast<float>(y)*delta.second;
  auto dir = (pixel_on_scr - position_).getNormalized();
  ray.setOrigin(position_);
  ray.setDirection(dir);
}

void Camera::calculateScreen()  {
  float rotation = get_rotation(rotation_angle_);
  float cos_up = cosf(rotation);
  float sin_up = sinf(rotation);

  bool is_zero_y_axis_with_dir_cross = false;
  linearmath::vec3<float> y_axis(0.0, 0.0, 1.0);
  //if (is_zero_y_axis_with_dir_cross)
  if (y_axis.cross(direction_).lengthSqr() == 0.0)
    y_axis = {0.0f,1.0f,0.0f};
  linearmath::vec3<float> x_axis = y_axis.cross(direction_);
  y_axis = direction_.cross(x_axis);
  
  scr_u_dir_ = x_axis;
  scr_v_dir_ = y_axis;

  upVector_ = x_axis*cos_up + y_axis*sin_up; // do i need this ? )
  // Calculate intersection of direction with screen plane
  auto intersection = position_ + direction_*camera_screen_distance_;
  // Claculate screen_origin
  auto intersection_scr_origin_dir = -(x_axis + y_axis).getNormalized();
  float scr_half_diagonal = camera_screen_distance_ * std::tanf(0.5f*fov_);
  scr_origin_ = intersection - intersection_scr_origin_dir*scr_half_diagonal;

  float coef = width_ / height_;
  scr_height_ = (scr_half_diagonal + scr_half_diagonal) / sqrtf(1 + coef*coef);
  scr_width_ = scr_height_ * coef;
}
