#include "decode_png.h"

#include <gtest/gtest.h>

#include <string>
#include <fstream>

namespace // anonymous
{

TEST(decode_png_test, general_test1)
{
    // Read data from PNG file
    std::string input_filename = "test/lion.png";
    std::ifstream fin(input_filename, std::ios::binary);

    ASSERT_TRUE(fin) << "\nCould not open " << input_filename << "\n";

    // Get length of file
    fin.seekg(0, fin.end);
    unsigned int length = fin.tellg();
    fin.seekg(0, fin.beg);

    uint8_t* png_data = new uint8_t[length];
    fin.read(reinterpret_cast<char*>(png_data), length);
    fin.close();

    std::cout << "Size of input png image is " << length << std::endl;

    size_t width = 0, height = 0;
    uint8_t num_channels = 0;
    std::vector<uint8_t> output_vector;
    int ret = decode_png(png_data, length, width, height, num_channels, output_vector);

    ASSERT_EQ(ret, 0);
}

} // end anonymous namespace