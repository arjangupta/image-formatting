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

int decode_png(uint8_t* png_data,
               size_t png_length,
               size_t &width, 
               size_t &height, 
               uint8_t &num_channels, 
               std::vector<uint8_t> output_vector)
{
    std::cout << "PNG DEBUG STATEMENT 1\n";

    if ( !png_check_sig(png_data, kPngSignatureLength) )
    {
        return 99;
    }

    // get PNG file info struct (memory is allocated by libpng)
    png_structp png_ptr = NULL;
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if(png_ptr == NULL)
    {
       return 98;
    }

    // get PNG image data info struct (memory is allocated by libpng)
    png_infop info_ptr = NULL;
    info_ptr = png_create_info_struct(png_ptr);

    if(info_ptr == NULL)
    {
       // libpng must free file info struct memory before we bail
       png_destroy_read_struct(&png_ptr, NULL, NULL);
       return 97;
    }

    return 0;
}
