#ifndef RPI_ST7735_VIGRA_INTERFACE_VIGRA_HXX
#define RPI_ST7735_VIGRA_INTERFACE_VIGRA_HXX

#include <vigra/multi_array.hxx>

#include "pixel_drawer/draw.hxx"


namespace rpi_st7735 {

EXIT_FLAG draw_multi_array(vigra::MultiArrayView<2, vigra::RGBValue<unsigned char> > image, const std::string& device_name);

EXIT_FLAG draw_multi_array(vigra::MultiArrayView<3, unsigned char> image, const std::string& device_name);

EXIT_FLAG draw_image_from_file(const std::string& filename, const std::string& device_name);

} // namespace rpi_st7735


#endif /* RPI_ST7735_VIGRA_INTERFACE_VIGRA_HXX */

