#include "_VectorTest.h"



_VectorTest::_VectorTest()
{
	this->_cycles = 1000;
}

void _VectorTest::Run()
{
	const char testHeader[] = "[----------------- Starting test cases for libvector module -------------------]\n \
----- It shoud test following classes :\n \
----- vector 2/3/4 d \n \
----- mat3x3 ";
	Profiler prf(testHeader, "", "C:\\CAA\\vector.txt", false);
	prf.start();
	prf.finish();
	this->_cmpVector2DEqual();
}

void _VectorTest::setCyclesCount(unsigned long cycles)
{
	this->_cycles = cycles;
}

unsigned long _VectorTest::getCyclesCount()
{
	return _cycles;
}


_VectorTest::~_VectorTest()
{
}

void _VectorTest::_cmpVector2DEqual()
{
	linearmath::vec2<float> a(5.6456f, -0.45234f);
	linearmath::vec2<float> b(5.6456f, -0.45221f);
	auto cycles = getCyclesCount();
	float result;

	// Origin vec2
	Profiler prf("Perfomance for origin method ", "FLT vec2d->length", "C:\\CAA\\vector.txt", true);
	prf.start();
	for (unsigned long i(0); i < cycles; ++i)
		result = a.length();
	prf.finish();

	// Modyfied vec2
	Profiler optPrf("Perfomance for modyf. method ", "FLT  vec2d->length", "C:\\CAA\\vector.txt", true);
	optPrf.start();
	a.invalidate();
	for (unsigned long i(0); i < cycles; ++i)
	{
		//a.invalidate();
		result = a.opt_length();
	}
	optPrf.finish();
}
