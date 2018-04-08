#include "decode_png.h"

extern "C"
{
    #include "png.h"
}

#include <iostream>

int decode_png(const std::vector<uint8_t> png_data, 
				size_t &width, 
				size_t &height, 
				uint8_t &num_channels, 
				std::vector<uint8_t> output_vector)
{
    std::cout << "Exercise the PNG library!" << std::endl;

    png_structp png_ptr = NULL;

    return 0;
}
