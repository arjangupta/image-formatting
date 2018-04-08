#!/bin/sh

# Remove build directory
rm -rf 3rdparty/build_png/
# Enter the directory of interest
cd 3rdparty/libpng
# If pnglibconf.h exists, remove it
rm pnglibconf.h
# Copy the pnglibconf.h.prebuilt and rename it
cp scripts/pnglibconf.h.prebuilt .
mv pnglibconf.h.prebuilt pnglibconf.h
# Execute the camke command
cmake -H. -B../build_png/ -DPNG_STATIC=True -DPNG_SHARED=True
# Run the build files
cd ../build_png/
make -j 4