cmake_minimum_required(VERSION 3.9)

project(image-formatting-ag)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -W -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall")

include_directories( src )

# Build library

add_library(decode-png STATIC src/decode_png.cpp )

# Test executable

add_executable( test-decode-png test/test_decode_png.cpp )
add_dependencies( test-decode-png decode_png )
target_link_libraries( test-decode-png decode_png )