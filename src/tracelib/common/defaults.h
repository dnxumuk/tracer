#pragma once
#include "../linearmath/vector3d.h"

extern linearmath::vec3<float> default_vacuum_color;// (0.0f, 0.0f, 0.15f);

extern linearmath::vec3<float> default_plane_color; //(0.0f, 1.0f, 0.0f);
extern linearmath::vec3<float> default_sphere_color; //(0.0f, 0.0f, 1.0f);
extern linearmath::vec3<float> default_face_color;// (1.0f, 1.0f, 0.0f);
extern linearmath::vec3<float> default_face_n_color;// (0.0f, 1.0f, 1.0f);

extern linearmath::vec3<float> default_star_color;// (0.5f, 1.0f, 1.0f);


extern size_t __non_star_chance;// = 9;
extern size_t default_stars_period;// = (1 << __non_star_chance) - 1;