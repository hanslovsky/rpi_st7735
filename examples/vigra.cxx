#include <iostream>

#include <vigra/multi_array.hxx>

#include "vigra_interface/vigra.hxx"

int main(int argc, char** argv) {
  char* device_name = "/dev/fb1";
  if (argc == 2) {
    device_name = argv[1];
  }

  vigra::MultiArray<2, vigra::RGBValue<unsigned char> > image(vigra::Shape2(128, 160));
  for (int x = 0; x < 128; ++x) {
    for (int y = 0; y < 160; ++y) {
      int color_index = 0;
      if (y < 55) {
        color_index = 0;
      } else if (y < 110) {
        color_index = 1;
      } else {
        color_index = 2;
      }
      image[vigra::Shape2(x, y)][0] = 0;
      image[vigra::Shape2(x, y)][1] = 0;
      image[vigra::Shape2(x, y)][2] = 0;
          
      image[vigra::Shape2(x, y)][color_index] = 255;
    }
  }

  std::cout << "All pixels set" << std::endl;
  return rpi_st7735::draw_multi_array(image, device_name);
}
