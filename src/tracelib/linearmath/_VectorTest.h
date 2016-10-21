#pragma once
#include "profiler.h"
#include "vector2d.h"

class _VectorTest
{
public:
	_VectorTest();
	void Run();
	void setCyclesCount(unsigned long cycles);
	unsigned long getCyclesCount();
	~_VectorTest();
private:
	unsigned long _cycles;
	void _cmpVector2DEqual();
};

