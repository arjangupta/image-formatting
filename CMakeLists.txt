cmake_minimum_required(VERSION 3.9)

# Include Hunter
include("cmake/HunterGate.cmake")
HunterGate(
  URL "https://github.com/ruslo/hunter/archive/v0.19.123.tar.gz"
  SHA1 "57d07480686f82ddc916a5980b4f2a18e5954c2b"
)

project(image-formatting-ag)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -W -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall")

# Get GTest
hunter_add_package(GTest)
find_package(GTest CONFIG REQUIRED)

# Get ZLIB
find_package(ZLIB REQUIRED)
include_directories(${ZLIB_INCLUDE_DIR})

# Get static libpng ready to be added
find_library(PNG_LIB_STATIC
    NAMES libpng16.a 
    PATHS ${CMAKE_CURRENT_LIST_DIR}/3rdparty/build_png/
    NO_DEFAULT_PATH
)

# Properly import the static libpng
include_directories(${CMAKE_CURRENT_LIST_DIR}/3rdparty/libpng/)
add_library( png STATIC IMPORTED GLOBAL )
set_target_properties( png PROPERTIES
    IMPORTED_LOCATION ${PNG_LIB_STATIC}
)

# Create our library
include_directories( src )
add_library(decode-png STATIC src/decode_png.cpp )

# Test executable
add_executable( test-decode-png test/test_decode_png.cpp )
add_dependencies( test-decode-png GTest::main ${ZLIB_LIBRARY} png decode-png)
target_link_libraries( test-decode-png GTest::main ${ZLIB_LIBRARY} png decode-png )