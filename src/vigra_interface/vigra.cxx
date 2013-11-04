#include <vigra/multi_array.hxx>
#include <vigra/impex.hxx>

#include "vigra_interface/vigra.hxx"

#include "pixel_drawer/draw.hxx"

namespace rpi_st7735 {


EXIT_FLAG draw_multi_array(vigra::MultiArrayView<2, vigra::RGBValue<unsigned char> > image, const std::string& device_name) {
  return write_sequence(device_name, image.begin(), image.end(), RGBLayoutInterpreter());
}

EXIT_FLAG draw_multi_array(vigra::MultiArrayView<3, unsigned char> image, const std::string& device_name) {
  return write_sequence(device_name, image.begin(), image.end(), FlatLayoutInterpreter());
}

EXIT_FLAG draw_image_from_file(const std::string& filename, const std::string& device_name) {
  vigra::ImageImportInfo info(filename.c_str());
  vigra::MultiArray<2, vigra::RGBValue<unsigned char> > image(info.shape());
  importImage(info, image);
  return draw_multi_array(image, device_name);
}

} // namespace rpi_st7735
