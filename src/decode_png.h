// Purpose: API header for decoding PNG images

#ifndef DECODE_PNG_H_
#define DECODE_PNG_H_

#include <vector>
#include <stdint.h>

int decode_png(uint8_t* png_data,
			   size_t png_length, 
               size_t &width, 
               size_t &height, 
               uint8_t &num_channels, 
               std::vector<uint8_t> output_vector);

#endif // DECODE_PNG_H_