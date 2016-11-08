#pragma once

#include <assert.h>
#include <math.h>
#include "fundamentalmath.h"

namespace linearmath
{
	template<typename T>
	class vec3 //final
	{
	public:
		T x, y, z;
		vec3();
		vec3(T const x, T const y, T const z);
    vec3(const vec3&rv) {
      x = rv.x;
      y = rv.y;
      z = rv.z;
    }

    inline bool vec3<T>::crossSign(vec3<T> const& v) const;

        bool     operator==(vec3<T> const& v) const;
		vec3<T>  operator+ (vec3<T> const& v) const;
		vec3<T>& operator+=(vec3<T> const& v);
		vec3<T>  operator- (vec3<T> const& v) const;
		vec3<T>& operator-=(vec3<T> const& v);
		vec3<T>  operator- () const;
		vec3<T>  operator* (float const f) const;
		vec3<T>  operator* (const linearmath::vec3<T>& v) const;
		vec3<T>& operator*=(float const f);
		vec3<T>  operator/ (float const f) const;
		vec3<T>& operator/=(float const f);

        float length() const;
		float lengthSqr() const;
		
        void    normalize();
		vec3<T> getNormalized() const;

        T dot(vec3<T> const& v) const;
		vec3  cross(vec3<T> const& v) const;

		float   distanceTo(vec3<T> const& p) const;
		float   angleWith(vec3<T> const& v) const;
		vec3<T> projectionOn(vec3<T> const& v) const;
		vec3<T> perpendicularOn(vec3 const& v) const;
	};
    //
    // vec3 class implementation
    //
	template<typename T>
	inline vec3<T>::vec3()
		: x(), y(), z()
	{
	}
	template<typename T>
	inline vec3<T>::vec3(T const x, T const y, T const z)
		: x(x), y(y), z(z)
	{
	}

	template<typename T>
	inline bool vec3<T>::operator==(vec3<T> const& v) const
	{
		return equals(this->x, v.x) && equals(this->y, v.y) && equals(this->z, v.z);
	}
	template<typename T>

	inline vec3<T> vec3<T>::operator+(vec3<T> const& v) const
	{
		return vec3<T>{this->x + v.x, this->y + v.y, this->z + v.z};
	}
	template<typename T>
	inline vec3<T>& vec3<T>::operator+=(vec3<T> const& v)
	{
		this->x += v.x;
		this->y += v.y;
		this->z += v.z;
		return *this;
	}

	template<typename T>
	inline vec3<T> vec3<T>::operator-(vec3<T> const& v) const
	{
		return vec3{this->x - v.x, this->y - v.y, this->z - v.z};
	}

	template<typename T>
	inline vec3<T>& vec3<T>::operator-=(vec3<T> const& v)
	{
		this->x -= v.x;
		this->y -= v.y;
		this->z -= v.z;
		return *this;
	}

	template<typename T>
	inline vec3<T> vec3<T>::operator-() const
	{
		return vec3<T>{-this->x, -this->y, -this->z};
	}

	template<typename T>
	inline vec3<T> vec3<T>::operator*(float const f) const
	{
		return vec3<T>{this->x * f, this->y * f, this->z * f};
	}

	template<typename T>
	inline vec3<T> vec3<T>::operator*(const linearmath::vec3<T>& v) const
	{
		return vec3<T>{ v.x * x, v.y * y, v.z * z };
	}

	template<typename T>
	inline vec3<T>& vec3<T>::operator*=(float const f)
	{
		this->x *= f;
		this->y *= f;
		this->z *= f;
		return *this;
	}

	template<typename T>
	inline vec3<T> vec3<T>::operator/(float const f) const
	{
		return vec3{this->x / f, this->y / f, this->z / f};
	}

	template<typename T>
	inline vec3<T>& vec3<T>::operator/=(float const f)
	{
		this->x /= f;
		this->y /= f;
		this->z /= f;
		return *this;
	}

	template<typename T>
	inline float vec3<T>::length() const
	{
		return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
	}

	template<typename T>
	inline float vec3<T>::lengthSqr() const
	{
		return this->x * this->x + this->y * this->y + this->z * this->z;
	}

	template<typename T>
	inline void vec3<T>::normalize()
	{
		float const length_reciprocal{1.0f / this->length()};
		this->x *= length_reciprocal;
		this->y *= length_reciprocal;
		this->z *= length_reciprocal;
	}

	template<typename T>
	inline vec3<T> vec3<T>::getNormalized() const
	{
		float length_reciprocal = 1.0f / this->length();
		return vec3<T>{this->x * length_reciprocal, this->y * length_reciprocal, this->z * length_reciprocal};
  }

	template<typename T>
	inline float vec3<T>::distanceTo(vec3<T> const& v) const
	{
		float const dist_x{v.x - this->x};
		float const dist_y{v.y - this->y};
		float const dist_z{v.z - this->z};
		return std::sqrt(dist_x * dist_x + dist_y * dist_y + dist_z * dist_z);
	}

	template<typename T>
	inline float vec3<T>::angleWith(vec3<T> const& v) const
	{
		return std::acos(this->dot(v) / (this->length() * v.length()));
	}

	template<typename T>
	inline vec3<T> vec3<T>::projectionOn(vec3<T> const& v) const
	{
		return v.normalized() * (this->dot(v) / v.length());
	}

	template<typename T>
	inline vec3<T> vec3<T>::perpendicularOn(vec3<T> const& v) const
	{
		return (*this) - this->projection_on(v);
	}

template<typename T>
    T vec3<T>::dot(vec3<T> const& v) const {
  T ret = x * v.x + y * v.y + z * v.z;
  return ret;
}

	template<typename T>
	inline vec3<T> vec3<T>::cross(vec3<T> const& v) const
	{
		return vec3<T>{this->y * v.z - this->z * v.y, this->z * v.x - this->x * v.z, this->x * v.y - this->y * v.x};
	}

  template<typename T>
  inline bool vec3<T>::crossSign(vec3<T> const& v) const {
    return( x*v.y - y*v.x ) > 0;
  }

	template<typename T>
	inline vec3<T> operator*(vec3<T> const& v)
	{
		return vec3<T>{ v.x*x, v.y*y, v.z*z };
	}
}