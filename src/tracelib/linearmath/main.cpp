#include <iostream>
#include "vector2d.h"
#include "vector3d.h"
#include "vector4d.h"
#include "mat3x3.h"
#include "Profiler.h"

#include "_VectorTest.h"

using namespace std;
using namespace vector;

int main3 ( int argc , char** argv )
{
/*
    cout << "Test cases :\n" << "1. Memory addresses for different vector types :\n";
    vec2<float> vect2( 10.f , 20.f );
    float *p2X , *p2Y;
    p2X = &vect2.x;
    p2Y = &vect2.y;
    cout << "For 2d vector we have : "<< p2X << " - " << p2Y << "\n";

    vec3<float> vect3( 10.f , 20.f, 30.f );
    float *p3X , *p3Y, *p3Z;
    p3X = &vect3.x;
    p3Y = &vect3.y;
    p3Z = &vect3.z;
    cout << "For 3d vector we have : "<< p3X << " - " << p3Y << " - " << p3Z << "\n";

    vec4<float> vect4( 10.f , 20.f, 30.f, 40.f );
    float *p4X , *p4Y, *p4Z , *p4W;
    p4X = &vect4.x;
    p4Y = &vect4.y;
    p4Z = &vect4.z;
    p4W = &vect4.w;
    cout << "For 4d vector we have : " << p4X << " - " << p4Y << " - " << p4Z << " - " << p4W << "\n";
*/   

/*
	Profiler prf ( "Multiplying matrix 3x3 10^7 times", "Matrix" , "C:\\CAA\\t.txt", true );

	const unsigned long count = 100'000'000;
	mat3x3<double>  a(  2.3, 2.4, 4.5,
				       -6.7, 6.7, 6.6,
				        4.2, 0.1, 7.7 );
	
	mat3x3<double> b( 3.4 ,   0.1,  45,
		              0.01,  -3.4,  67,
		              3.4 ,   102,  46.5 );

	mat3x3<double> result;
	prf.start();
	for (size_t i(0); i < count; i++)
		result = a*b;

    prf.finish();
*/
	_VectorTest test;
	test.setCyclesCount(1000 * 1000);
	test.Run();

    int fg;
    cin >> fg;
	return 1;
}