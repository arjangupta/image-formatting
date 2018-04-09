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
               std::vector<uint8_t> &output_vector)
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

    png_uint_32 width_uint32 = 0;
    png_uint_32 height_uint32 = 0;
    int bit_depth = 0;
    int colorType = -1;
    png_uint_32 retval = png_get_IHDR(png_ptr, info_ptr,
                                      &width_uint32,
                                      &height_uint32,
                                      &bit_depth,
                                      &colorType,
                                      NULL, NULL, NULL);

    if(retval != 1)
    {
       return 96; // TODO: Add error enums & cleanup
    }

    // TODO: Decide if we want to support 16 bit color.
    // If we want to support it, we should wait till
    // we can use the Image PixelFormat/DataLayout enums
    // in this image-compression interface.
    if(bit_depth == 16)
    {
        // TODO: Remove this cout
        std::cout << "Converting to 8 bit_depth" << std::endl;
        png_set_strip_16(png_ptr);
    }

    // TODO: Need unit test case wherever we have couts
    switch (colorType) // TODO: Remove couts
    {
        case PNG_COLOR_TYPE_GRAY:
            std::cout << "colorType is grayscale." << std::endl;
            if (bit_depth < 8)
            {
                png_set_expand_gray_1_2_4_to_8(png_ptr);
            }
            
            // Expand tRNS chunks to alpha
            if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
            {
                // TODO: Remove cout
                std::cout << "Expanding tRNS chunks to alpha channel" << std::endl;
                png_set_tRNS_to_alpha(png_ptr);
                num_channels = 2;
            }
            else
            {
                num_channels = 1;
            }
            break;

        case PNG_COLOR_TYPE_GRAY_ALPHA:
            std::cout << "colorType is grayscale alpha." << std::endl;
            // No expansion needed - gray-alpha is always 8 or 16 bit depth
            num_channels = 2;
            break;

        case PNG_COLOR_TYPE_RGB:
            std::cout << "colorType is RGB." << std::endl;
            // Expand tRNS chunks to alpha
            if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
            {
                // TODO: Remove cout
                std::cout << "Expanding tRNS chunks to alpha channel" << std::endl;
                png_set_tRNS_to_alpha(png_ptr);
                num_channels = 4;
            }
            else
            {
                num_channels = 3;
            }
            break;

        case PNG_COLOR_TYPE_PALETTE:
            png_set_palette_to_rgb(png_ptr);
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

    // TODO: Move these couts to test exec
    std::cout << "The bit depth is: " << bit_depth << std::endl;
    width = width_uint32;
    std::cout << "The width is: " << width << std::endl;
    height = height_uint32;
    std::cout << "The height is: " << height << std::endl;

    // These color_type don't have an alpha channel then fill it with 0xff.
    // if(color_type == PNG_COLOR_TYPE_RGB ||
    // color_type == PNG_COLOR_TYPE_GRAY ||
    // color_type == PNG_COLOR_TYPE_PALETTE)
    // png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER);

    // if(color_type == PNG_COLOR_TYPE_GRAY ||
    // color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    // png_set_gray_to_rgb(png_ptr);

    png_read_update_info(png_ptr, info_ptr);

    const png_uint_32 bytes_per_row = png_get_rowbytes(png_ptr, info_ptr);

    if ( bytes_per_row != width * num_channels )
    {
        return 94;
    }

    // Get output_vector ready to store data
    output_vector.reserve(width * height * num_channels * (bit_depth/8));

    png_bytep* row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
    for(size_t y = 0; y < height; y++)
    {
        row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png_ptr, info_ptr));
    }

    png_read_image(png_ptr, row_pointers);

    // Write to vector
    for (size_t y = 0; y < height; ++y)
    {
        output_vector.insert(output_vector.end(), row_pointers[y], row_pointers[y] + png_get_rowbytes(png_ptr, info_ptr));
    }

    // Clean up
    for(size_t y = 0; y < height; y++) {
        free(row_pointers[y]);
    }
    free(row_pointers);

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return 0;
}
