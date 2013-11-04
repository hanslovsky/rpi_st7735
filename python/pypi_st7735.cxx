#define PY_ARRAY_UNIQUE_SYMBOL pypi_st7735_pyarray

#include <Python.h>

#include <boost/python.hpp>

#include <vigra/numpy_array.hxx>
#include <vigra/multi_shape.hxx>
#include <vigra/numpy_array_converters.hxx>

#include "vigra_interface/vigra.hxx"

using namespace rpi_st7735;
using namespace boost::python;

void export_vigra_interface();


EXIT_FLAG py_draw_from_file(const std::string& filename, const std::string& device_name) {
  return draw_image_from_file(filename, device_name);
}
  

EXIT_FLAG py_draw_multi_array(vigra::NumpyArray<3, unsigned char> image, const std::string& device_name) {
  return draw_multi_array(image, device_name);
}


void export_vigra_interface() {
  boost::python::enum_<EXIT_FLAG>("EXIT_FLAG")
      .value("SUCCESS", SUCCESS)
      .value("BUFFER_OPEN_ERROR", BUFFER_OPEN_ERROR)
      .value("IOCTL_ERROR", IOCTL_ERROR)
      .value("INCONSISTENCY_ERROR", INCONSISTENCY_ERROR)
      ;

  def("drawFromFile", &py_draw_from_file);
  def("drawMultiArray", vigra::registerConverters(&py_draw_multi_array));
}
  



BOOST_PYTHON_MODULE( pi_st7735 ) {
  vigra::import_vigranumpy();
  export_vigra_interface();
}
