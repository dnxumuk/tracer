#pragma once

#include <memory.h>

class Imagemap
{
public:
	Imagemap();
	Imagemap( size_t width, size_t height );
	~Imagemap();

	unsigned char* getData() const { return _imgData; };
	size_t  width()  const { return _width; };
	size_t  height() const { return _height; };
	void reset();
private:
	unsigned char *_imgData;
	size_t         _width;
	size_t         _height;
};

