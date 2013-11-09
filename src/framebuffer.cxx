// stl
#include <string>
#include <iostream>

// sys
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

// linux
#include <linux/fb.h>

// file descriptor
#include <unistd.h>
#include <fcntl.h> // O_RDWR

// own
#include "framebuffer/framebuffer.hxx"
// #include "framebuffer/pixelsetter.hxx"
// #include "framebuffer/exception.hxx"

namespace framebuffer {

////
//// class Framebuffer
////
Framebuffer::Framebuffer(const std::string& device) :
    buffer_(nullptr),
    setter_(nullptr) {
  info_.device = device;
  info_.file_descriptor = open(info_.device.c_str(), O_RDWR);
  if (info_.file_descriptor >= 0) {
    fb_var_screeninfo var_info;
    fb_fix_screeninfo fix_info;
    if (!ioctl(info_.file_descriptor, FBIOGET_VSCREENINFO, &var_info) &&
        !ioctl(info_.file_descriptor, FBIOGET_FSCREENINFO, &fix_info)) {
      info_.buffer_length = var_info.yres_virtual * fix_info.line_length;
      info_.bits_per_pixel = var_info.bits_per_pixel;
      info_.bytes_per_pixel = info_.bits_per_pixel/8;
      info_.width = var_info.xres;
      info_.height = var_info.yres;
      info_.line_length = fix_info.line_length;
      info_.offset_x = var_info.xoffset;
      info_.offset_y = var_info.yoffset;
      std::cout << "length=" << info_.buffer_length << ", bytes_per_pixel=" << info_.bytes_per_pixel
                << ", width=" << info_.width << ",height=" << info_.height
                << ", line_length=" << info_.line_length 
                << ", xoffset=" << info_.offset_x << ", yoffset=" << info_.offset_y
                << std::endl;
      buffer_ = (char*) mmap(NULL,
                             info_.buffer_length,
                             PROT_WRITE,
                             MAP_SHARED,
                             info_.file_descriptor,
                             0);
    } else {
      // throw IOCTL_ERROR;
    }
  } else {
    // throw BUFFER_OPEN_ERROR;
  }
  if (info_.bytes_per_pixel == 4) {
    setter_ = std::unique_ptr<PixelSetter>(new PixelSetter_32bit);
    info_.number_of_channels = 4; // GET INFO FROM ioctl?
  } else if (info_.bytes_per_pixel == 2) {
    setter_ = std::unique_ptr<PixelSetter>(new PixelSetter_16bit);
    info_.number_of_channels = 3; // GET INFO FROM ioctl?
  } else {
    // throw NOT IMPLEMENTED ERROR
  }
}


Framebuffer::~Framebuffer() {
  if (buffer_ != nullptr) {
    munmap(buffer_, info_.buffer_length);
  }
  if (info_.file_descriptor >= 0) {
    close(info_.file_descriptor);
  }
  std::cout << "DESTRUCTION ALL AROUND ME" << std::endl;
}

} // namespace framebuffer



