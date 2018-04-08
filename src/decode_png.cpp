#include "decode_png.h"

extern "C"
{
    #include "png.h"
}

#include <iostream>

namespace
{

const size_t kPngSignatureLength = 8;

}

int decode_png(const std::vector<uint8_t> png_data, 
               size_t &width, 
               size_t &height, 
               uint8_t &num_channels, 
               std::vector<uint8_t> output_vector)
{
    png_structp png_ptr = NULL;

    if ( !png_check_sig(png_data.data(), kPngSignatureLength) )
    {
        return 99;
    }

    return 0;
}
