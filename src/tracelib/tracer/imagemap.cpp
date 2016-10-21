#include "imagemap.h"



Imagemap::Imagemap()
{
}

Imagemap::Imagemap(size_t width, size_t height)
{
	// Allocating memory for RGBA-imagemap ( 4 channels by 1 byte per channel );
	_width  = width;
	_height = height;
	_imgData = new unsigned char[ width*height*4];
}

Imagemap::~Imagemap()
{
	delete[] _imgData;
}

void Imagemap::reset()
{
	const int value = 0;
	memset(_imgData, value, (_width*_height*4) / sizeof(value));
}
