#pragma once

#include <assert.h>
#include "fundamentalmath.h"

namespace linearmath
{
    template<typename T>
	class vec4 //final
	{
	    public:
		    T x, y, z, w;
		    vec4();
		    vec4( T const x, T const y, T const z, T const w );
    };
    // 
    // vec4 class implementation
    //
	template<typename T>
	inline vec4<T>::vec4()
		: x(), y(), z(), w()
	{
	}
	template<typename T>
	inline vec4<T>::vec4(T const x, T const y, T const z, T const w )
		: x(x), y(y), z(z), w(w)
	{
	}
}