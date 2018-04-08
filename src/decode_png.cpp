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

    png_read_info(png_ptr, info_ptr);

    png_uint_32 width_uint = 0;
    png_uint_32 height_uint = 0;
    int bitDepth = 0;
    int colorType = -1;
    png_uint_32 retval = png_get_IHDR(png_ptr, info_ptr,
                                      &width_uint,
                                      &height_uint,
                                      &bitDepth,
                                      &colorType,
                                      NULL, NULL, NULL);

    if(retval != 1)
    {
       return 96; // TODO: Add error enums & cleanup
    }

    switch (colorType) // TODO: Remove couts
    {
        case PNG_COLOR_TYPE_GRAY:
            std::cout << "colorType is grayscale." << std::endl;
            num_channels = 1;
            break;

        case PNG_COLOR_TYPE_GRAY_ALPHA:
            std::cout << "colorType is grayscale alpha." << std::endl;
            num_channels = 2;
            break;

        case PNG_COLOR_TYPE_RGB:
            std::cout << "colorType is RGB." << std::endl;
            num_channels = 3;
            break;

        case PNG_COLOR_TYPE_RGBA:
            std::cout << "colorType is RGBA." << std::endl;
            num_channels = 4;
            break;

        default:
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            return 95;
    }

    // TODO: Remove these couts
    std::cout << "The bit depth is: " << bitDepth << std::endl;
    width = width_uint;
    std::cout << "The width is: " << width << std::endl;
    height = height_uint;
    std::cout << "The height is: " << height << std::endl;

    const png_uint_32 bytes_per_row = png_get_rowbytes(png_ptr, info_ptr);
    uint8_t* row_data = new uint8_t[bytes_per_row];

    // Get output_vector ready to store data
    output_vector.reserve(width * height * num_channels);

    // We will read a single row at a time
    for (size_t row_index = 0; row_index < height; ++row_index)
    {
        png_read_row(png_ptr, (png_bytep)row_data, NULL);

        const size_t start_of_current_row = row_index * width;

        for (size_t column_index = 0; column_index < width; ++column_index)
        {
            for (size_t channel_index = 0; channel_index < num_channels; ++channel_index)
            {
                output_vector.push_back(start_of_current_row + column_index + channel_index);
            }
        }
    }

    delete[] row_data;

    return 0;
}
