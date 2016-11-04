#include "imagemap.h"

const size_t bitsPerPixel = 3;


Imagemap::Imagemap() {
}

Imagemap::Imagemap(size_t width, size_t height) {
  // Allocating memory for RGBA-imagemap ( 4 channels by 1 byte per channel );
  width_ = width;
  height_ = height;
  imgData_ = new unsigned char[width*height *bitsPerPixel];
}

Imagemap::~Imagemap() {
  delete[] imgData_;
}

void Imagemap::reset() {
  const int value = 0;
  memset(imgData_, value, (width_*height_*bitsPerPixel) / sizeof(value));
}
