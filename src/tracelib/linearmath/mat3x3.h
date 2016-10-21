#pragma once
#include <cmath>
#include "vector3d.h"

namespace linearmath
{
    template <typename T>
	class mat3x3 // final
	{
	    public:
		    T data[3][3]; //
    		mat3x3();     //
    		mat3x3( T const dataArray[3][3]);  //
    		mat3x3( T const m00, T const m01, T const m02,
			        T const m10, T const m11, T const m12,
			        T const m20, T const m21, T const m22
                  ); //
		    mat3x3<T> operator*( mat3x3<T> const& m ) const;   //
			vec3<T>   operator*( vec3<T> const& m) const;   //

    		T det() const; //
    		mat3x3 inversed() const; //
    		mat3x3 inversed_precalc_det( T const det) const;  //
		    static mat3x3 scale( T const x, T const y, T const z); //
    		static mat3x3 uniform_scale( T const s);               //
		    static mat3x3 rotation_around_x_axis( T const radians); //
		    static mat3x3 rotation_around_y_axis( T const radians);
    		static mat3x3 rotation_around_z_axis( T const radians);
    		static mat3x3 rotation_around_axis( vec3<T> const& axis, T const radians);
    		static const mat3x3 IDENTITY;
	};
    //
    // mat3x3 class implementation
    //
	template<typename T>
	mat3x3<T>::mat3x3()
	{

	}

    template<typename T>
    mat3x3<T>::mat3x3 ( T const dataArray[3][3] )
    {
        for ( size_t i(0) ; i < 9 ; ++i )
                data[i] = dataArray[i];
    }

    template<typename T>
    mat3x3<T>::mat3x3 ( T const m00, T const m01, T const m02,
			            T const m10, T const m11, T const m12,
			            T const m20, T const m21, T const m22
                    )
    {
        data[0][0] = m00 ; data[0][1] = m01 ; data[0][2] = m02;
        data[1][0] = m10 ; data[1][1] = m11 ; data[1][2] = m12;
        data[2][0] = m20 ; data[2][1] = m21 ; data[2][2] = m22;
    }

    // Multiply two 3x3 matixes
    // OPTIMIZE IT !!!!!!!!!!!!!!
    template<typename T>
    mat3x3<T> mat3x3<T>::operator*( mat3x3 const& mat ) const
    {
	    mat3x3<T> result;
	    for ( size_t i(0); i < 3; ++i)
		    for ( size_t j(0); j < 3; ++j)
		    {
			    result.data[i][j] = 0.0f;
			    for (size_t k(0); k < 3; ++k)
				    result.data[i][j] += this->data[i][k] * mat.data[k][j];
		    }
	    return result;
    }

	template<typename T>
	inline vec3<T> mat3x3<T>::operator*( vec3<T> const & m) const
	{

		return vec3<T>(
			m.x * data[0][0] + m.y * data[0][1] + m.z * data[0][2],
			m.x * data[1][0] + m.y * data[1][1] + m.z * data[1][2],
			m.x * data[2][0] + m.y * data[2][1] + m.z * data[2][2]
			);
	}

    template<typename T>
    T mat3x3<T>::det() const
    {
        T result;
		data[0][0] * ( data[1][1] * data[2][2] - data[1][2] * data[2][1] ) -
		data[0][1] * ( data[1][0] * data[2][2] - data[1][2] * data[2][0] ) +
		data[0][2] * ( data[1][0] * data[2][1] - data[1][1] * data[2][0] );
    }

    template<typename T>
    mat3x3<T> mat3x3<T>::inversed() const
    {
	    return inversed_precalc_det( det() );
    }
    
    template<typename T>
    mat3x3<T> mat3x3<T>::inversed_precalc_det( T const det ) const
    {
	    mat3x3 result;

	    result.values[0][0] = data[1][1] * data[2][2] - data[1][2] * data[2][1];
	    result.values[1][0] = data[1][2] * data[2][0] - data[1][0] * data[2][2];
	    result.values[2][0] = data[1][0] * data[2][1] - data[1][1] * data[2][0];

	    result.values[0][1] = data[0][2] * data[2][1] - data[0][1] * data[2][2];
	    result.values[1][1] = data[0][0] * data[2][2] - data[0][2] * data[2][0];
	    result.values[2][1] = data[0][1] * data[2][0] - data[0][0] * data[2][1];

	    result.values[0][2] = data[0][1] * data[1][2] - data[0][2] * data[1][1];
	    result.values[1][2] = data[0][2] * data[1][0] - data[0][0] * data[1][2];
	    result.values[2][2] = data[0][0] * data[1][1] - data[0][1] * data[1][0];
	    
        T const det_ivnersed = static_cast<T>( 1.0 ) / det;
	    for ( size_t j(0); j < 3; ++j )
		    for (size_t i{0}; i < 3; ++i)
			    result.values[i][j] *= det_ivnersed;
    	return result;
    }

    template<typename T>
    mat3x3<T> mat3x3<T>::scale( T const x, T const y, T const z)
    {
        const T zero = static_cast<T>( 0.0 );
	    return matrix3x3f (
		                    x,    zero, zero,
		                    zero, y,    zero,
		                    zero, zero, z
                           );
    }

    template<typename T>
    mat3x3<T> mat3x3<T>::uniform_scale( T const scale )
    {
        const T zero = static_cast<T>( 0.0 );
	    return mat3x3f (
		                    scale, zero,  zero,
		                    zero,  scale, zero,
		                    zero,  zero,   scale
                           );
    }

    // Try to optimize by delivering the computing sin && cos values into T type values. I may prevent the similar computing twice
    // -> These refers to 3 functions implemented below with names like rotation_around_%_axis
    template<typename T>
    mat3x3<T> mat3x3<T>::rotation_around_x_axis( T const radians)
    {
        const T zero = static_cast<T>( 0.0 );
        const T one  = static_cast<T>( 1.0 );
		T sinus = sin(radians);
		T cosinus = cos(radians);
	    return mat3x3 (
		                one ,  zero ,        zero,
		                zero,  cosinus, sinus,
		                zero, -sinus, cosinus
                       );
    }
    

    template<typename T>
    mat3x3<T> mat3x3<T>::rotation_around_y_axis( T const radians)
    {
        const T zero = static_cast<T>( 0.0 );
        const T one  = static_cast<T>( 1.0 );
	    return mat3x3 (
		                cos(radians), zero , -std::sin(radians),
		                zero,         one,    zero,
		                sin(radians), zero,   cos(radians)
                       );
    }
    
    template<typename T>
    
    mat3x3<T> mat3x3<T>::rotation_around_z_axis( T const radians)
    {
        const T zero = static_cast<T>( 0.0 );
        const T one  = static_cast<T>( 1.0 );
	    return mat3x3 (
		                cos(radians), sin(radians) , zero,
		               -sin(radians), cos(radians),  zero,
		                zero,         zero,          one
                       );
    }

    template<typename T>
    mat3x3<T> mat3x3<T>::rotation_around_axis( vec3<T> const& axis, T const radians)
    {
	    vec3<T> axis( axis.normalized() );

        const T zero = static_cast<T>( 0.0 );
        const T one  = static_cast<T>( 1.0 );
        const T cos  = cos(radians);
    	const T sin  = sin(radians);

        return mat3x3 (
                        axis.x * axis.x * ( one - cos ) + cos,  
                        axis.x * axis.y * ( one - cos ) + axis.z * sin, 
                        axis.x * axis.z * ( one - cos ) - axis.y * sin,

                        axis.x * axis.y * ( one - cos ) - axis.z * sin,
                        axis.y * axis.y * ( one - cos ) + cos,
                        axis.y * axis.z * ( one - cos ) + axis.x * sin,

                        axis.x * axis.z * ( one - cos ) + axis.y * sin,
                        axis.y * axis.z * ( one - cos ) - axis.x * sin,
                        axis.z * axis.z * ( one - cos ) + cos,
                      );
    }
}
