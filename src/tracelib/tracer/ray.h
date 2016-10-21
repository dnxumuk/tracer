#pragma once
#include "..\linearmath\vector3d.h"


class Ray
{
public:
	Ray();
	Ray( const linearmath::vec3<float>& origin, const linearmath::vec3<float>& direction) ;
	~Ray();

	linearmath::vec3<float> getOriginPoint() const;
	linearmath::vec3<float> getDirection()   const;

	linearmath::vec3<float> getInvDirection() const;

	void setDirection(const linearmath::vec3<float>& dir);
private:
	linearmath::vec3<float> _origin;
    // Direction is normalized
	linearmath::vec3<float> _direction;
	linearmath::vec3<float> _invDirection;
};

