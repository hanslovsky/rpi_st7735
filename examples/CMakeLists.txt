ADD_EXECUTABLE(vigra_example vigra.cxx)
ADD_EXECUTABLE(random_example random.cxx)

TARGET_LINK_LIBRARIES(vigra_example vigra_interface)
TARGET_LINK_LIBRARIES(random_example vigra_interface)   
