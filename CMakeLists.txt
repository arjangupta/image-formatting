cmake_minimum_required(VERSION 3.9)

project(image-formatting-ag)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -W -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall")

#get vcpkg path
set(VCPKG_PATH $ENV{VCPKG_PATH})
if (NOT VCPKG_PATH)
  message(FATAL_ERROR "Set VCPKG_PATH environment variable")
endif()

#include vcpkg toolchain and add include dirs
include(${VCPKG_PATH}/scripts/buildsystems/vcpkg.cmake)
include_directories(${VCPKG_PATH}/installed/x64-osx/include)

# Get packages
find_package(OpenCV REQUIRED)
if (NOT OpenCV_FOUND)
  message(ERROR "OpenCV not found.")
endif()

find_package(GTest REQUIRED)
if (NOT GTest_FOUND)
  message(ERROR "GTest not found.")
endif()

find_package(JPEG REQUIRED)
if (NOT JPEG_FOUND)
  message(ERROR "JPEG not found.")
endif()

find_package(ZLIB REQUIRED)
if (NOT ZLIB_FOUND)
  message(ERROR "ZLIB not found.")
endif()

find_package(PNG REQUIRED)
if (NOT PNG_FOUND)
  message(ERROR "PNG not found.")
endif()

# Create our library
include_directories( src )
add_library(decode-png STATIC src/decode_png.cpp )

# Add path to test_dir
add_definitions(-DDATA_DIR="${CMAKE_SOURCE_DIR}/test/")

# Test executable
add_executable( test-decode-png 
  test/test_decode_png.cpp
)
add_dependencies( test-decode-png 
  GTest::Main
  ZLIB::ZLIB
  PNG::PNG
  decode-png
)
target_link_libraries( test-decode-png 
  GTest::Main
  ZLIB::ZLIB
  PNG::PNG
  decode-png
)

# Executable for the openCV ppm test data generator
add_executable( test_data_generator 
  src/test_data_generator.cpp
)
add_dependencies(test_data_generator
  ${OpenCV_LIBS}
)
target_link_libraries(test_data_generator
  ${OpenCV_LIBS}
)

#Executable for the openCV scaling test data generator
add_executable(opencv_scaling_generator src/opencv_scaling_generator)
add_dependencies(opencv_scaling_generator ${OpenCV_LIBS})
target_link_libraries(opencv_scaling_generator ${OpenCV_LIBS})
