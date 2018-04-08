#include "decode_png.h"

extern "C"
{
    #include "png.h"
}

#include <iostream>

namespace
{

const size_t kPngSignatureLength = 8;

struct read_buffer_struct 
{ 
    const uint8_t* data; 
    size_t bytes_left; 
};

void read_data_from_input_png(png_structp png_ptr, png_bytep outBytes,
   png_size_t byteCountToRead)
{
    read_buffer_struct* read_ptr = static_cast<read_buffer_struct*>(png_get_io_ptr(png_ptr));
    if (byteCountToRead > read_ptr->bytes_left)
    {
        png_error(png_ptr, "read error");
    }
    memcpy(outBytes, read_ptr->data, byteCountToRead);
    read_ptr->bytes_left -= byteCountToRead;
    std::cout << "bytes left to be read: " << read_ptr->bytes_left << std::endl;
    read_ptr->data += byteCountToRead;
}

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

    // Obtain PNG file info struct - memory is allocated by libpng
    png_structp png_ptr = NULL;
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if(png_ptr == NULL)
    {
       return 98;
    }

    // Obtain PNG image data info struct - memory is allocated by libpng
    png_infop info_ptr = NULL;
    info_ptr = png_create_info_struct(png_ptr);

    if(info_ptr == NULL)
    {
       // libpng must handle memory freeing
       png_destroy_read_struct(&png_ptr, NULL, NULL);
       return 97;
    }

    read_buffer_struct read_ptr;
    read_ptr.data = png_data + 8;
    read_ptr.bytes_left = png_length - 8;
    png_set_read_fn(png_ptr, &read_ptr, read_data_from_input_png);

    // tell libpng we already read the signature
    png_set_sig_bytes(png_ptr, kPngSignatureLength);

    return 0;
}
