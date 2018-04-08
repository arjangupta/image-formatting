#include <gtest/gtest.h>

#include "decode_png.h"

namespace // anonymous
{

TEST(decode_png_test, general_test1)
{
    decode_png();
    ASSERT_TRUE(true);
}

} // end anonymous namespace