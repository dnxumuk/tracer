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
  void setCameraToScreenDistance(float distance);
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
    //const float half_diagonal = tanf(fov_/2.0f);
    //const float w = ;
    //const float h = ;
    return std::make_pair(scr_width_/width_, scr_height_/height_);

    //const float  min = -1.0f * cameraDist;
    //const float  max = 1.0f * cameraDist;
    //float aspect = width / static_cast<float>(height);

   // float dX = (max - min) / static_cast<float>(width) * aspect;
   // float dY = (max - min) / static_cast<float>(height);

  }
  float getAspect() const {
    return aspect_;
  }
  float getCameraToScreenDistance() const;
  void setAspect(float aspect) {
    aspect_ = aspect;
	calculateScreen();
  }
//private:
public:
  // Resolution and screen position
  size_t width_;
  size_t height_;

  // Screen position and directions of u and v
  linearmath::vec3<float> scr_origin_;
  linearmath::vec3<float> scr_u_dir_;
  linearmath::vec3<float> scr_v_dir_;
  float scr_width_;
  float scr_height_;


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

