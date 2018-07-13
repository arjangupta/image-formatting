//Generate scaled images using opencv, to use as test data for custom scaling algorithm

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <string>
#include <vector>


void opencv_scale(std::string image_dir, std::string file_name, size_t dst_cols, 
                size_t dst_rows, std::string output_dir, int interpolation_type)
{
    cv::Mat input_mat = cv::imread(image_dir + file_name);

    cv::Size dst_size(dst_cols, dst_rows);
    cv::Mat output_mat;
    cv::resize(input_mat, output_mat, dst_size, interpolation_type);

    std::vector<int> imwrite_params;
    imwrite_params.push_back(CV_IMWRITE_JPEG_QUALITY);
    imwrite_params.push_back(100);
    cv::imwrite(output_dir + file_name, output_mat, imwrite_params);
}

int main()
{
    std::string test_input_dir = DATA_DIR;
    test_input_dir.append("opencv_scale_input/");

    std::string output_path = DATA_DIR;
    output_path.append("opencv_scale_output/");

    //earth.jpg is 1248 × 1280
    opencv_scale(test_input_dir, "earth.jpg", 624, 640, output_path, cv::INTER_LINEAR);
    opencv_scale(test_input_dir, "earth.jpg", 624, 640, output_path, cv::INTER_NEAREST);

    return 0;
}