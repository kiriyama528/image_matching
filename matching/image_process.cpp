#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "image_process.h"
#include "define.h"

using namespace cv;

bool readImage(const char* imgname, Mat &mat) {
	mat = imread(imgname);

	if (mat.empty()) {
		fprintf(stderr, " ERROR : ‰æ‘œ‚Ì“Ç‚İ‚İ‚É¸”s‚µ‚Ü‚µ‚½B\n");
		fprintf(stderr, "       file name : %s\n", imgname);
		return false;
	}

	return true;
}


bool getImgInfo(const Mat & img, UTYPE * rows, UTYPE * cols, UTYPE * channels) {
	if (img.empty()) {
		return false;
	}

	*rows = img.rows;
	*cols = img.cols;
	*channels = img.channels();

	return true;
}
