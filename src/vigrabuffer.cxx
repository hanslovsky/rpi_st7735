// stdl
#include <string>

// vigra
#include <vigra/multi_array.hxx>

// own
#include "framebuffer/framebuffer.hxx"
#include "framebuffer/vigrabuffer.hxx"

namespace framebuffer {

////
//// VigraBuffer
////
VigraBuffer::VigraBuffer(const std::string& device) :
    buffer_(device) {

}


ReturnStatus VigraBuffer::draw_rectangle(vigra::MultiArrayView<3, unsigned char> rectangle,
                                         size_t top_left_x,
                                         size_t top_left_y) {
  return buffer_.draw_rectangle(&(*rectangle.begin()),
                                &(*rectangle.end()),
                                top_left_x,
                                top_left_y,
                                rectangle.shape()[1],
                                rectangle.shape()[2]
                                );
}


 const Framebuffer::Info& VigraBuffer::info() const {
  return buffer_.info();
}

}
