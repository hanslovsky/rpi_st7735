// stl
#include <cstdint>

// own
#include "framebuffer/pixelsetter.hxx"

namespace framebuffer {

int PixelSetter_32bit::draw(char* buffer, unsigned char* src) {
  // assume no alpha channel
  buffer[0] = src[2];
  buffer[1] = src[1];
  buffer[2] = src[0];
  buffer[3] = 0;
  return 0;
}


int PixelSetter_16bit::draw(char* buffer, unsigned char* src) {
  // lots of casting and bit shifts:
  // R - first 5 bits
  // G - middle 6 bits
  // B - last 5 bits
  *(int16_t*)(buffer) =
      (int)src[2] << RED_SHIFT |
      (int)src[1] << RED_SHIFT + GREEN_SHIFT |
      (int)src[0] << RED_SHIFT + GREEN_SHIFT + BLUE_SHIFT
      ;
  return 0;
}

} // namespace framebuffer




