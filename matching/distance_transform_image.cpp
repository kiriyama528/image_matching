#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "image_process.h"
#include "define.h"
#include "distance_transform_image.h"

void makeDistanceTransformImage(cv::Mat &dst, const cv::Mat &src) {
	UTYPE rows, cols, channels;
	getImgInfo(src, &rows, &cols, &channels);
	dst = cv::Mat::zeros(rows, cols, CV_8UC1);

	// TODO À‘•‚ğì‚êB¡‚ÍRED
}