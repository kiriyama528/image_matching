#include "pch.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "distance_transform_image.h"
#include "define.h"
#include "image_process.h"

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

TEST(UnitTestDistanceTransformImage, DTI) {
	const int rows = 6, cols = 6;
		
	// エッジ画像として使う場合、1以外は0に置き換えて利用する
	// [0]channel目に書き込む
	unsigned char v[rows*cols] = {
		0, 1, 1, 1, 1, 1,
		1, 2, 2, 2, 2, 1,
		0, 1, 2, 3, 2, 1,
		0, 1, 2, 2, 2, 1,
		0, 0, 1, 1, 1, 0,
		0, 0, 0, 0, 0, 0
	};

	cv::Mat src = cv::Mat::zeros(6, 6, CV_8UC1);
	unsigned char *data = src.data;
	// エッジ画像を用意する
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < rows; c++) {
			int idx = r * cols + c; // channel数は1channelに決め打ち
			if (v[idx] == 1) {
				data[idx] = v[idx];
			}
		}
	}
	
	cv::Mat actual;
	makeDistanceTransformImage(actual, src);

	cv::Mat expected(rows, cols, CV_8UC1);
	memcpy(expected.data, v, rows*cols);

	// 行数、列数、チャネル数は一致していることを前提にしている
	EXPECT_TRUE(isEqualMat(expected, actual));
}
