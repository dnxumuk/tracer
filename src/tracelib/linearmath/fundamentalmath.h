#pragma once

#include <math.h>
using namespace std;

namespace vector
{
	float const FLOAT_EPSILON = 0.00001f;

	template<typename T>
	inline bool equals( T const x, T const y)
	{
		return abs(x - y) < static_cast<T>(FLOAT_EPSILON);
	}
}