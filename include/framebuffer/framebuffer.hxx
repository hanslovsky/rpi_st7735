#ifndef RPI_ST7735_FRAMEBUFFER_FRAMEBUFFER_HXX
#define RPI_ST7735_FRAMEBUFFER_FRAMEBUFFER_HXX

// stl
#include <string>
#include <memory>
#include <iostream>

// own
#include "framebuffer/pixelsetter.hxx"

namespace framebuffer {
enum class ReturnStatus {SUCCESS=0, INCONSISTENCY_ERROR, OUT_OF_RANGE};

class Framebuffer {
 public:
  struct Info {
    size_t buffer_length;
    int file_descriptor;
    size_t width;
    size_t height;
    size_t offset_x;
    size_t offset_y;
    size_t bits_per_pixel;
    size_t bytes_per_pixel;
    size_t number_of_channels;
    size_t line_length;
    std::string device;
  };
  Framebuffer() = delete;
  explicit Framebuffer(const std::string& device);
  ~Framebuffer();
  template <typename Iterator>
  ReturnStatus draw_rectangle(Iterator begin,
                              Iterator end,
                              size_t top_left_x,
                              size_t top_left_y,
                              size_t width,
                              size_t height);
  const Info& info() const;
                     
 private:
  char* buffer_;
  std::unique_ptr<PixelSetter> setter_;
  Info info_;
};

template <typename Iterator>
ReturnStatus Framebuffer::draw_rectangle(Iterator begin,
                                         Iterator end,
                                         size_t top_left_x,
                                         size_t top_left_y,
                                         size_t width,
                                         size_t height) {
  if (top_left_x + width > info_.width ||
      top_left_y + height > info_.height) {
    return ReturnStatus::OUT_OF_RANGE;
  }
  for (size_t y = 0; y < height; ++y) {
    char* buffer = buffer_
        + (info_.offset_y + top_left_y + y)*info_.line_length
        + (top_left_x + info_.offset_x)*info_.bytes_per_pixel
        ;
    for (size_t x = 0; x < width; ++x) {
      setter_->draw(buffer, reinterpret_cast<char*>(begin));
      buffer += info_.bytes_per_pixel;
      begin += info_.number_of_channels;
    }
  }
  return ReturnStatus::SUCCESS;
}

} // namespace framebuffer

#endif /* RPI_ST7735_FRAMEBUFFER_FRAMEBUFFER_HXX */
