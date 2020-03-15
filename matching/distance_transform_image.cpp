#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "image_process.h"
#include "define.h"
#include "distance_transform_image.h"

void makeDistanceTransformImage(const cv::Mat &src, cv::Mat dst) {
	UTYPE rows, cols, channels;
	getImgInfo(src, &rows, &cols, &channels);
	dst.create(rows, cols, CV_8UC1);
}