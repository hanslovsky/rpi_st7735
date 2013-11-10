FIND_PACKAGE(Boost REQUIRED COMPONENTS python)
FIND_PACKAGE(PythonLibs REQUIRED) #  PATHS "/usr/lib/libpython2.7.so")
FIND_PACKAGE(PythonInterp REQUIRED) #  PATHS "/usr/bin/python2")
FILE(GLOB PYTHON_MODULE "*.cxx")
ADD_LIBRARY(pi_st7735 SHARED ${PYTHON_MODULE})
SET_TARGET_PROPERTIES(pi_st7735 PROPERTIES PREFIX "" OUTPUT_NAME pi_st7735 SUFFIX ".so")
TARGET_LINK_LIBRARIES(pi_st7735 vigra_interface framebuffer ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
# INSTALL(TARGETS pi_st7735 LIBRARY DESTINATION lib/python2.7/site-packages)
INSTALL(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}" DESTINATION "lib/python2.7/site-packages"
  USE_SOURCE_PERMISSIONS
  REGEX "cmake" EXCLUDE
  REGEX "CMake" EXCLUDE
  REGEX "Makefile" EXCLUDE
  REGEX "pyc" EXCLUDE
)
INCLUDE_DIRECTORIES(SYSTEM "${PYTHON_INCLUDE_DIRS}")

#--------------------------------------------------------------
# Copy from src to build
#--------------------------------------------------------------

if( ${CMAKE_CURRENT_SOURCE_DIR} STREQUAL  ${CMAKE_CURRENT_BINARY_DIR} )
  #message(STATUS "same src and build dir.")
else()
  #message(STATUS "copy python-opengm files  from src to build" )
  file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
endif()