#ifndef RPI_ST7735_PIXEL_DRAWER_DRAW_HXX
#define RPI_ST7735_PIXEL_DRAWER_DRAW_HXX

#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include <linux/fb.h>

#include <unistd.h>
#include <fcntl.h>

#include <iostream>

namespace rpi_st7735 {


enum EXIT_FLAG {SUCCESS, BUFFER_OPEN_ERROR, IOCTL_ERROR, INCONSISTENCY_ERROR};

const int CHANNELS = 3;
const int PIXEL_DEPTH = 8;

const int shifts[] = {0, 5, 11};

                 
struct BufferInfo {
  size_t buflen;
  struct fb_var_screeninfo var_info;
  struct fb_fix_screeninfo fix_info;
};

                 
// int write_at(char* buffer, size_t x, size_t y, const std::tuple<char, char, char>& rgb, const BufferInfo& info);


class RGBLayoutInterpreter {
 public:
  template <typename Iterator>
  EXIT_FLAG write_sequence(char* buffer, Iterator begin, Iterator end, const BufferInfo& info) {
    size_t stepsize = info.var_info.bits_per_pixel/8;
    std::cout << info.buflen << std::endl;
    std::cout << end-begin << std::endl;
    if (info.buflen != stepsize*(end-begin)) {
      return (INCONSISTENCY_ERROR);
    }
    for (; begin != end; ++begin, buffer += stepsize) {
      *(int16_t*)(buffer) =
          (int)(*begin)[0] << shifts[0] |
          (int)(*begin)[1] << shifts[1] |
          (int)(*begin)[2] << shifts[2]
          ;
    }
    return SUCCESS;
  }
};


class FlatLayoutInterpreter {
 public:
  template <typename Iterator>
  EXIT_FLAG write_sequence(char* buffer, Iterator begin, Iterator end, const BufferInfo& info) {
    size_t stepsize = info.var_info.bits_per_pixel/8;
    size_t channels = 3;
    std::cout << info.buflen << std::endl;
    std::cout << end-begin << std::endl;
    std::cout << info.buflen << ',' << channels*1.0/stepsize*(end-begin) << std::endl;
    if (channels*1.0/stepsize*info.buflen != 1.0*(end-begin)) {
      return (INCONSISTENCY_ERROR);
    }
    for (; begin != end; begin += channels, buffer += stepsize) {
      *(int16_t*)(buffer) =
          (int)(*begin+0) << shifts[0] |
          (int)(*begin+1) << shifts[1] |
          (int)(*begin+2) << shifts[2]
          ;
    }
    return SUCCESS;
  }
};



template <typename Iterator, typename LayoutInterpreter>
EXIT_FLAG write_sequence(const std::string& filename, Iterator begin, Iterator end, LayoutInterpreter interpreter) {
  char* buffer = NULL;
  BufferInfo info;
  int file_descriptor = -1;
  EXIT_FLAG flag = BUFFER_OPEN_ERROR;
  file_descriptor = open(filename.c_str(), O_RDWR);

  if (file_descriptor >= 0) {
    if (!ioctl(file_descriptor, FBIOGET_VSCREENINFO, &info.var_info) &&
        !ioctl(file_descriptor, FBIOGET_FSCREENINFO, &info.fix_info)) {
      info.buflen = info.var_info.yres_virtual * info.fix_info.line_length;
      buffer = (char*) mmap(NULL,
                            info.buflen,
                            PROT_WRITE,
                            MAP_SHARED,
                            file_descriptor,
                            0);
      flag = interpreter.write_sequence(buffer, begin, end, info);
    } else {
      flag = IOCTL_ERROR;
    }
  } else {
    flag = BUFFER_OPEN_ERROR;
  }

  if (file_descriptor >= 0) {
    close(file_descriptor);
  }
  return flag;
}



} // namespace rpi_st7735

#endif /* RPI_ST7735_PIXEL_DRAWER_DRAW_HXX */
