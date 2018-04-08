cmake_minimum_required(VERSION 3.9)

project(image-formatting-ag)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -W -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall")

include_directories( src )

add_library(decode-png STATIC src/decode_png.cpp )

add_executable(  )
