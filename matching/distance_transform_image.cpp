#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "image_process.h"
#include "define.h"
#include "distance_transform_image.h"
#include "contour_tracking.h"


void makeDistanceTransformImage(cv::Mat &dst, const cv::Mat &src) {
	UTYPE rows, cols, channels;
	getImgInfo(src, &rows, &cols, &channels);
	dst = cv::Mat::zeros(rows, cols, CV_8UC1);

	/*
	// TODO é¿ëïÇçÏÇÍÅBç°ÇÕGREEN
	unsigned char v[36] = {
		0, 1, 1, 1, 1, 1,
		1, 2, 2, 2, 2, 1,
		0, 1, 2, 3, 2, 1,
		0, 1, 2, 2, 2, 1,
		0, 0, 1, 1, 1, 0,
		0, 0, 0, 0, 0, 0
	};
	memcpy(dst.data, v, 36);
	*/

}