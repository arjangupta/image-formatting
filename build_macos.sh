rm -rf build_macos/
cmake -H. -Bbuild_macos
cd build_macos/
make -j 4
