#define PY_ARRAY_UNIQUE_SYMBOL pypi_st7735_pyarray

// stl
#include <string>

// python
#include <Python.h>

// boost
#include <boost/python.hpp>

// vigra
#include <vigra/numpy_array.hxx>
#include <vigra/multi_array.hxx>
#include <vigra/multi_shape.hxx>
#include <vigra/numpy_array_converters.hxx>

// own
#include "vigra_interface/vigra.hxx"
#include "framebuffer/framebuffer.hxx"
#include "framebuffer/vigrabuffer.hxx"

using namespace rpi_st7735;
using namespace boost::python;

void export_vigra_interface();
void export_framebuffer();

EXIT_FLAG py_draw_from_file(const std::string& filename, const std::string& device_name) {
  return draw_image_from_file(filename, device_name);
}
  

EXIT_FLAG py_draw_multi_array(vigra::NumpyArray<3, unsigned char> image, const std::string& device_name) {
  return draw_multi_array(vigra::MultiArrayView<3,unsigned char>(image), device_name);
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


class PyVigraBuffer {
 public:
  explicit PyVigraBuffer(const std::string& device) :
      vigra_buffer_(device) {
  }

  // explicit PyVigraBuffer(const PyVigraBuffer& other) :
  //     vigra_buffer_("/dev/fb0") {
  // }
  
  framebuffer::ReturnStatus draw_rectangle(vigra::NumpyArray<3, unsigned char> rectangle,
                                           size_t top_left_x,
                                           size_t top_left_y) {
    vigra::MultiArrayView<3, unsigned char> copy(rectangle);
    return vigra_buffer_.draw_rectangle(copy, top_left_x, top_left_y);
  }
  const framebuffer::Framebuffer::Info& info() const {
    return vigra_buffer_.info();
  }
 private:
  framebuffer::VigraBuffer vigra_buffer_;
};

typedef framebuffer::Framebuffer::Info PyFrameBuffer;

void export_framebuffer() {
  boost::python::enum_<framebuffer::ReturnStatus>("ReturnStatus")
      .value("SUCCES", framebuffer::ReturnStatus::SUCCESS)
      .value("INCONSISTENCY_ERROR", framebuffer::ReturnStatus::INCONSISTENCY_ERROR)
      .value("OUT_OF_RANGE", framebuffer::ReturnStatus::OUT_OF_RANGE)
      ;
  
  class_<PyVigraBuffer, boost::noncopyable>("VigraBuffer", init<std::string>())
      .def("drawRectangle", vigra::registerConverters(&PyVigraBuffer::draw_rectangle))
      .def("getInfo", &PyVigraBuffer::info, return_internal_reference<>())
      ;

  class_<PyFrameBuffer, boost::noncopyable>("BufferInfo")
      .def_readonly("width", &PyFrameBuffer::width)
      .def_readonly("height", &PyFrameBuffer::height)
      .def_readonly("device", &PyFrameBuffer::device)
      ;
}
  



BOOST_PYTHON_MODULE( pi_st7735 ) {
  Py_Initialize();
  vigra::import_vigranumpy();
  export_vigra_interface();
  export_framebuffer();
}
