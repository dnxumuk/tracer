#pragma once

#include <assert.h>
#include "fundamentalmath.h"

namespace linearmath
{
	template<typename T>
	class vec2  // final
	{
	    public:
		    T x,y;
			T _length;

		vec2();
		vec2(T const x, T const y);
		inline   void     invalidate();

		bool     operator==(vec2<T> const& v) const;
		vec2<T>  operator+ (vec2<T> const& v) const;
		vec2<T>& operator+=(vec2<T> const& v);
		vec2<T>  operator- (vec2<T> const& v) const;
		vec2<T>& operator-=(vec2<T> const& v);
		vec2<T>  operator- () const;
		vec2<T>  operator* (float const f) const;
		vec2<T>& operator*=(float const f);
		vec2<T>  operator/ (float const f) const;
		vec2<T>& operator/=(float const f);

        T     length() const;
		T     opt_length() const;
		T     lengthSqr() const;

		void    normalize();
		vec2<T> getNormalized() const;

		float dot(vec2<T> const& v) const;
		float distanceTo(vec2<T> const& v) const;
		float angleWith(vec2<T> const& v) const;

		vec2<T> projectionOn(vec2<T> const& v) const;
		vec2<T> perpendicularOn(vec2 const& v) const;
	};
//
//  Iimplementation of vec2 class
//
    template<typename T>
	inline vec2<T>::vec2()
		: x(), y()
	{
	}
	template<typename T>
	inline vec2<T>::vec2(T const x, T const y)
		: x(x), y(y)
	{
	}

	template<typename T>
	inline void vec2<T>::invalidate()
	{
		_length = std::sqrt(this->x * this->x + this->y * this->y);
	}

    // Try Xor or byte comparation to speed up
	template<typename T>
	inline bool vec2<T>::operator==(vec2<T> const& v) const
	{
		return ( this->x & v.x );
	}

	template<typename T>
	inline vec2<T> vec2<T>::operator+(vec2<T> const& v) const
	{
		return vec2<T>{this->x + v.x, this->y + v.y};
	}
    // Try to compare with vec.Add( vec1 )
	template<typename T>
	inline vec2<T>& vec2<T>::operator+=(vec2<T> const& v)
	{
		this->x += v.x;
		this->y += v.y;
		return *this;
	}

	template<typename T>
	inline vec2<T> vec2<T>::operator-(vec2<T> const& v) const
	{
		return vec2{this->x - v.x, this->y - v.y};
	}

	template<typename T>
	inline vec2<T>& vec2<T>::operator-=(vec2<T> const& v)
	{
		this->x -= v.x;
		this->y -= v.y;
		return *this;
	}

	template<typename T>
	inline vec2<T> vec2<T>::operator-() const
	{
		return vec2<T>{-this->x, -this->y};
	}

	template<typename T>
	inline vec2<T> vec2<T>::operator*(float const f) const
	{
		return vec2<T>{this->x * f, this->y * f};
	}

	template<typename T>
	inline vec2<T>& vec2<T>::operator*=(float const f)
	{
		this->x *= f;
		this->y *= f;
		return *this;
	}

	template<typename T>
	inline vec2<T> vec2<T>::operator/(float const f) const
	{
		return vec2{this->x / f, this->y / f};
	}

	template<typename T>
	inline vec2<T>& vec2<T>::operator/=(float const f)
	{
		this->x /= f;
		this->y /= f;
		return *this;
	}

	// Optimize by precalculate
	template<typename T>
	inline T vec2<T>::length() const
	{
		return std::sqrt(this->x * this->x + this->y * this->y);
	}
	//
	template<typename T>
	inline T vec2<T>::opt_length() const
	{
		return _length;
	}

	template<typename T>
	inline T vec2<T>::lengthSqr() const
	{
		return this->x * this->x + this->y * this->y;
	}
    
	template<typename T>
	inline void vec2<T>::normalize()
	{
		float const length_reciprocal{1.0f / this->length()};
		x *= length_reciprocal;
		y *= length_reciprocal;
	}

	template<typename T>
	inline vec2<T> vec2<T>::getNormalized() const
	{
		float const length_reciprocal{1.0f / this->length()};
		return vec2<T>{this->x * length_reciprocal, this->y * length_reciprocal};
	}

	template<typename T>
	inline float vec2<T>::dot(vec2<T> const& v) const
	{
		return this->x * v.x + this->y * v.y;
	}

	template<typename T>
	inline float vec2<T>::distanceTo(vec2<T> const& v) const
	{
		float const dist_x{v.x - this->x};
		float const dist_y{v.y - this->y};

		return std::sqrt(dist_x * dist_x + dist_y * dist_y);
	}

	template<typename T>
	inline float vec2<T>::angleWith(vec2<T> const& v) const
	{
		return std::acos(this->dot(v) / (this->length() * v.length()));
	}

	template<typename T>
	inline vec2<T> vec2<T>::projectionOn(vec2<T> const& v) const
	{
		return v.normalized() * (this->dot(v) / v.length());
	}

	template<typename T>
	inline vec2<T> vec2<T>::perpendicularOn(vec2<T> const& v) const
	{
		return (*this) - this->projection_on(v);
	}
//
//	// Typedefs
//
//	using vec2f = vec2<float>;
//	using vec2ui = vec2<unsigned int>;
}
