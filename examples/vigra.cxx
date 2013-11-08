#include <iostream>

#include <vigra/multi_array.hxx>

#include "vigra_interface/vigra.hxx"

int main(int argc, char** argv) {
  char* device_name = "/dev/fb1";
  if (argc == 2) {
    device_name = argv[1];
  }

  int sum = 0;
  vigra::MultiArray<2, vigra::RGBValue<unsigned char> > image(vigra::Shape2(128, 160));
  vigra::MultiArray<3, unsigned char> image3(vigra::Shape3(3, 128, 160));
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
      image3(0, x, y) = 0;
      image3(1, x, y) = 0;
      image3(1, x, y) = 0;
          
      image3(color_index, x, y) = (unsigned char)255;
      sum += 255;
    }
  }
  std::cout << sum << std::endl;
  std::cout << "All pixels set" << std::endl;
  std::cout << "(0,0,0) "  << ", (0,0,1) " 
            << " diff: " << &image3(0,0,1) - &image3(0,0,0) << std::endl;
  std::cout << "begin "  << ", begin+1 " 
            << "diff: " << &(*(image3.begin()+1)) - &(*(image3.begin())) << std::endl;
  return rpi_st7735::draw_multi_array(image3, device_name);
}
