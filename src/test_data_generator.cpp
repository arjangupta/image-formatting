/* Generate test data for a directory (path) given as input to executable */

// OpenCV headers
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// Other cpp std lib headers
#include <iostream>
#include <string>
#include <vector>

void generate_test_data(std::string test_input_dir, std::string sub_dir, std::string file_name)
{
	std::string full_path = test_input_dir + sub_dir + "/" + file_name + ".png";
	cv::Mat png_image = cv::imread(full_path);
	
	// Dump data to test gen folder
	std::string test_generation_dir = DATA_DIR;
	test_generation_dir.append("generated_test_data/");
	std::vector<int> imwrite_params;
	imwrite_params.push_back(CV_IMWRITE_PXM_BINARY);
	imwrite_params.push_back(1);
	cv::imwrite((test_generation_dir + sub_dir + "/" + file_name + ".ppm"), png_image, imwrite_params);
}

void encode_test_data()
{
	std::cout << "Hello Solar System" << std::endl;
}

int main()
{
	std::string test_input_dir = DATA_DIR;
	test_input_dir.append("decode_png_input/");

	generate_test_data(test_input_dir, "gray", "gray-baseball");
	generate_test_data(test_input_dir, "gray", "gray-parrot");
	generate_test_data(test_input_dir, "gray-alpha", "gray-alpha1");
	generate_test_data(test_input_dir, "palette", "2-bit");
	generate_test_data(test_input_dir, "palette", "clouds");
	generate_test_data(test_input_dir, "palette", "three-leaves");
	generate_test_data(test_input_dir, "rgb", "graydient");
	generate_test_data(test_input_dir, "rgb", "rgb-base");
	generate_test_data(test_input_dir, "rgb", "bald-eagle");
	generate_test_data(test_input_dir, "rgba", "lion");
	generate_test_data(test_input_dir, "rgba", "ink-splash");
	
	return 0;
}