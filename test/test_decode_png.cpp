#include "decode_png.h"

#include <gtest/gtest.h>

#include <string>
#include <fstream>

namespace // anonymous
{

TEST(decode_png_test, general_test1)
{
    // Read data from PNG file
    std::string input_filename = "test/data/png_input/lion.png";
    std::ifstream fin(input_filename, std::ios::binary);

    ASSERT_TRUE(fin) << "Could not open "

    // Get length of file
    fin.seekg(0, fin.end);
    unsigned int length = fin.tellg();
    fin.seekg(0, fin.beg);

    std::vector<uint8_t> png_data;
    png_data.reserve(length);
    fin.read(reinterpret_cast<char*>(png_data.data()), length);
    fin.close();

    size_t width = 0, height = 0;
    uint8_t num_channels = 0;
    std::vector<uint8_t> output_vector;
    int ret = decode_png(png_data, width, height, num_channels, output_vector);

    ASSERT_EQ(ret, 0);
}

} // end anonymous namespace