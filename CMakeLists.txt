cmake_minimum_required(VERSION 3.9)

# Include Hunter
include("cmake/HunterGate.cmake")
HunterGate(
  URL "https://github.com/ruslo/hunter/archive/v0.19.123.tar.gz"
  SHA1 "57d07480686f82ddc916a5980b4f2a18e5954c2b"
)

project(image-formatting-ag)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -W -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall")

# Get GTest
hunter_add_package(GTest)
find_package(GTest CONFIG REQUIRED)
hunter_add_package(OpenCV)
find_package(OpenCV CONFIG REQUIRED)

# Get static zlib ready to be added
find_library( Z_LIB_STATIC
  NAMES libz.a
  PATHS ${CMAKE_CURRENT_LIST_DIR}/3rdparty/prebuilts/
  NO_DEFAULT_PATH
)

# Get static libpng ready to be added
find_library(PNG_LIB_STATIC
    NAMES libpng16.a 
    PATHS ${CMAKE_CURRENT_LIST_DIR}/3rdparty/build_png/
    NO_DEFAULT_PATH
)

# Properly import the static libz
include_directories(${CMAKE_CURRENT_LIST_DIR}/3rdparty/zlib/)
add_library( z STATIC IMPORTED GLOBAL )
set_target_properties( z PROPERTIES
  IMPORTED_LOCATION ${Z_LIB_STATIC}
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

# Add path to test_dir
add_definitions(-DDATA_DIR="${CMAKE_SOURCE_DIR}/test/")

# Test executable
add_executable( test-decode-png test/test_decode_png.cpp )
add_dependencies( test-decode-png GTest::main z png decode-png)
target_link_libraries( test-decode-png GTest::main z png decode-png )

# Executable for the openCV test data generator
add_executable( test_data_generator src/test_data_generator.cpp )
add_dependencies( test_data_generator ${OpenCV_LIBS} )
target_link_libraries( test_data_generator ${OpenCV_LIBS} )
