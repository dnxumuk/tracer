#pragma once
#include <memory.h>

class Imagemap{
public:
  Imagemap();
  Imagemap(size_t width, size_t height);
  ~Imagemap();

  unsigned char* getData() const { 
    return imgData_;
  };
  size_t width() const { 
    return width_;
  };
  size_t height() const { 
    return height_;
  };

  float getMegapixels() const {
    return 1e-6*width_*height_;
  }

  void reset();
private:
  unsigned char *imgData_;
  size_t width_;
  size_t height_;
};

