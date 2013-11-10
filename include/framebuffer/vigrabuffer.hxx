#ifndef RPI_ST7735_FRAMEBUFFER_VIGRABUFFER_HXX
#define RPI_ST7735_FRAMEBUFFER_VIGRABUFFER_HXX

// stl
#include <string>

// own
#include "framebuffer/framebuffer.hxx"

namespace vigra {
template <unsigned int N, typename T, typename TAG>
class MultiArrayView;
} // namespace vigra

namespace framebuffer {
class Framebuffer;
enum class ReturnStatus;

class VigraBuffer {
 public:
  VigraBuffer() = delete;
  explicit VigraBuffer(const std::string& device);
  ReturnStatus draw_rectangle(vigra::MultiArrayView<3, unsigned char> rectangle,
                                           size_t top_left_x,
                                           size_t top_left_y);
  const Framebuffer::Info& info() const;
 private:
  Framebuffer buffer_;
};

}

#endif /* RPI_ST7735_FRAMEBUFFER_VIGRABUFFER_HXX */

