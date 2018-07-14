//Generate scaled images using opencv, to use as test data for custom scaling algorithm

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

bool writeVectorToFile(std::string const& file_path,
                       cv::Mat const& input_data)
{
    //Open file
    std::ofstream write_stream(file_path);
    if (!write_stream)
    {
        return false;
    }

    //Check validity of vector
    if ( input_data.total() == 0 )
    {
        return false;
    }
    
    //Copy ByteSequence to file
    std::copy(input_data.ptr(),
              input_data.ptr() + (input_data.cols * 
                                  input_data.rows * 
                                  input_data.elemSize()),
              std::ostreambuf_iterator<char>(write_stream));
    write_stream.close();
    
    //Check file closure
    if (write_stream.fail())
    {
        return false;
    }

    return true;
}

void opencvScale(std::string image_path, size_t dst_cols, size_t dst_rows,
                 std::string output_path, int interpolation_type)
{
    cv::Mat input_mat = cv::imread(image_path);

    cv::Size dst_size(dst_cols, dst_rows);
    cv::Mat output_mat;
    cv::resize(input_mat, output_mat, dst_size, interpolation_type);

    writeVectorToFile(output_path, output_mat);
}

int main()
{
    std::string test_input_dir = DATA_DIR;
    test_input_dir.append("opencv_scale_input/");

    std::string output_path = DATA_DIR;
    output_path.append("opencv_scale_output/");

    //earth.jpg is 1248 × 1280
    //bilinear
    opencvScale(test_input_dir + "earth.jpg", 624, 640, 
                 output_path + "earth_halfscale_bilinear.data", cv::INTER_LINEAR);
    //nearest
    opencvScale(test_input_dir + "earth.jpg", 624, 640,
                 output_path + "earth_halfscale_nearestneighbor.data", cv::INTER_NEAREST);

    return 0;
}