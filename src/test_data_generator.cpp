/* Generate test data for a directory (path) given as input to executable */

// OpenCV headers
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// System includes for checking paths
#include <sys/types.h>
#include <sys/stat.h>

// C-header include for directory stream
#include <dirent.h>

// Other cpp std lib headers
#include <iostream>
#include <string>
#include <vector>

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