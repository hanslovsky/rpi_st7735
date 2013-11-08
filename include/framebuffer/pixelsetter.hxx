#ifndef RPI_ST7735_FRAMEBUFFER_PIXELSETTER_HXX
#define RPI_ST7735_FRAMEBUFFER_PIXELSETTER_HXX

namespace framebuffer {

class PixelSetter {
 public:
  virtual int draw(char* buffer, char* src) = 0;
};

class PixelSetter_32bit : public PixelSetter {
 public:
  virtual int draw(char* buffer, char* src);
};

class PixelSetter_8bit : public PixelSetter {
 public:
  enum {RED_SHIFT = 5, GREEN_SHIFT = 6, BLUE_SHIFT = 5};
  virtual int draw(char* buffer, char* src);
};

} // namespace framebuffer

#endif /* RPI_ST7735_FRAMEBUFFER_PIXELSETTER_HXX */
