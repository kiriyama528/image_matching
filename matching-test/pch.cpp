//
// pch.cpp
// Include the standard header and generate the precompiled header.
//

#include "pch.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

bool isEqualMat(const cv::Mat mat1, const cv::Mat mat2) {
	if (mat1.rows != mat2.rows
		|| mat1.cols != mat2.cols
		|| mat1.channels() != mat2.channels()
		|| mat1.type() != mat2.type())
	{
		return false;
	}
	return memcmp(mat1.data, mat2.data, mat1.rows * mat2.cols) == 0;
}