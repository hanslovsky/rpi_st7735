#ifndef RPI_ST7735_FRAMEBUFFER_FRAMEBUFFER_HXX
#define RPI_ST7735_FRAMEBUFFER_FRAMEBUFFER_HXX

// stl
#include <string>
#include <memory>

// own
#include "framebuffer/pixelsetter.hxx"

namespace framebuffer {

class Framebuffer {
 public:
  struct Info {
    size_t buffer_length;
    int file_descriptor;
    int width;
    int height;
    int bits_per_pixel;
    int bytes_per_pixel;
    std::string device;
  };
  Framebuffer() = delete;
  explicit Framebuffer(const std::string& device);
  ~Framebuffer();
  template <typename Iterator>
  int draw_rectangle(Iterator begin,
                     Iterator end,
                     size_t top_left_x,
                     size_t top_left_y,
                     size_t width,
                     size_t height);
                     
 private:
  char* buffer_;
  std::unique_ptr<PixelSetter> setter_;
  Info info_;
};

} // namespace framebuffer

#endif /* RPI_ST7735_FRAMEBUFFER_FRAMEBUFFER_HXX */
