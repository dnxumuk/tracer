#pragma once
#include "../linearmath/vector3d.h"
#include <tuple>

class Ray;

class Camera {
public:
  Camera();
  ~Camera();
  // Getters
  linearmath::vec3<float> getRay(size_t x, size_t y, Ray &ray) const;
  std::pair<float, float> getDxDY() const {
    const float half_diagonal = tanf(fov_/2.0f);
    const float w = std::sqrtf(4*half_diagonal*half_diagonal/(1.0f + aspect_*aspect_));
    const float h = w * aspect_;
    return std::make_pair(w/width_, h/height_);

    //const float  min = -1.0f * cameraDist;
    //const float  max = 1.0f * cameraDist;
    //float aspect = width / static_cast<float>(height);

   // float dX = (max - min) / static_cast<float>(width) * aspect;
   // float dY = (max - min) / static_cast<float>(height);

  }
  float getAspect() const {
    return aspect_;
  }
  std::pair<size_t, size_t> getResolution() const {
    return std::make_pair(width_,height_);
  }
  linearmath::vec3<float> getPosition() const {
    return position_;
  }
  linearmath::vec3<float> getDirection() const {
    return direction_;
  }
  float getCameraScreenDistance() const {
    return camera_screen_distance_;
  }
  // Setters
  void setCameraScreenDistance(float distance) {
    camera_screen_distance_ = distance;
  }
  void setResolution(size_t width, size_t height) {
    width_ = width;
    height_ = height;
  }
  void setPosition(const linearmath::vec3<float> &position,
                   const linearmath::vec3<float> &direction) {
    position_ = position;
    direction_ = direction;
  }
  void setPosition(const linearmath::vec3<float> &position) {
    position_ = position;
  }
  void setLookAt(const linearmath::vec3<float> &lookAtPoint) {
    direction_ = (lookAtPoint - position_).getNormalized();
  }
  void setRotation(float angle) {
  rotation_angle_ = angle;
  }
  void setAspect(float aspect) {
    aspect_ = aspect;
  }
private:
  // Resolution
  size_t width_;
  size_t height_;
  // Camera position
  linearmath::vec3<float> position_;
  linearmath::vec3<float> direction_;
  linearmath::vec3<float> upVector_;
  // Camera angle
  float fov_;
  float aspect_; // Example 16:9 ~ 1.75 w/h
  //
  // Other parameters
  float rotation_angle_;
  float camera_screen_distance_;

};

