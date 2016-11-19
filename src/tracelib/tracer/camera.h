#pragma once
#include "../linearmath/vector3d.h"
#include <tuple>

class Ray;

class Camera {
public:
  Camera();
  ~Camera();
  // Getters
  void getRay(size_t x, size_t y, Ray &ray) const;
  void calculateScreen();
  // FOV
  void setFOVDegrees(float degrees);
  void setFOVRadians(float rads);
  float getFOV(bool in_degrees = true);
  // Resolution
  void setResolution(size_t width, size_t height);
  std::pair<size_t, size_t> getResolution() const;
  // Camera position
  void setCameraScreenDistance(float distance);
  void setPosition(const linearmath::vec3<float> &position,
                   const linearmath::vec3<float> &direction);
  void setPosition(const linearmath::vec3<float> &position);
  void setLookAt(const linearmath::vec3<float> &lookAtPoint);
  void setRotation(float angle);
  
  linearmath::vec3<float> getPosition() const;
  linearmath::vec3<float> getDirection() const;

  //

  // Sly ....
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
  float getCameraScreenDistance() const;
  void setAspect(float aspect) {
    aspect_ = aspect;
  }
private:
  // Resolution and screen position
  size_t width_;
  size_t height_;
  linearmath::vec3<float> screen_position_;
  linearmath::vec3<float> screen_x_dir_;
  linearmath::vec3<float> screen_y_dir_;
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

