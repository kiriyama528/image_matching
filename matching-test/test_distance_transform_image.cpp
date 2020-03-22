#include "pch.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "distance_transform_image.h"
#include "define.h"
#include "image_process.h"



class UnitTestDistanceTransformImage : public ::testing::Test {
protected:
	// エッジ画像として使う場合、1以外は0に置き換えて利用する
	// [0]channel目に書き込む
	unsigned char v[7 * 7] = {
		0, 0, 0, 0, 0, 0, 0,
		0, 1, 1, 1, 1, 1, 0,
		1, 2, 2, 2, 2, 1, 0,
		0, 1, 2, 3, 2, 1, 0,
		0, 1, 2, 2, 2, 1, 0,
		0, 0, 1, 1, 1, 0, 0,
		0, 0, 0, 0, 0, 0, 0
	};

	void toEdge(cv::Mat &dst, const cv::Mat &src) {
		// エッジ画像を用意する
		int rows = src.rows;
		int cols = src.cols;
		dst = cv::Mat::zeros(rows, cols, CV_8UC1);
		unsigned char *dst_data = dst.data;
		unsigned char *src_data = src.data;
		for (int r = 0; r < rows; r++) {
			for (int c = 0; c < rows; c++) {
				int idx = r * cols + c; // channel数は1channelに決め打ち
				if (src_data[idx] == 1) {
					dst_data[idx] = src_data[idx];
				}
			}
		}

	}

	void toEdge(cv::Mat &dst, const unsigned char src[], int rows, int cols) {
		cv::Mat src_mat(rows, cols, CV_8UC1);
		memcpy(src_mat.data, src, rows * cols);
		toEdge(dst, src_mat);
	}

	virtual void SetUp() {
		// do nothing
	}

	virtual void TearDown() {
		// do nothing
	}
};


TEST_F(UnitTestDistanceTransformImage, DTI) {
	const int rows = 7, cols = 7;
	unsigned char v_1[rows*cols];
	for (int i = 0; i < rows*cols; i++) {
		v_1[i] = (0 < v[i]) ? 1 : 0;
	}

	cv::Mat bi_img = cv::Mat::zeros(rows, cols, CV_8UC1);
	memcpy(bi_img.data, v_1, rows*cols);
	
	cv::Mat actual;
	makeDistanceTransformImage(actual, bi_img); // making 今はスタブ状態

	cv::Mat expected(rows, cols, CV_8UC1);
	memcpy(expected.data, v, rows*cols);

	EXPECT_TRUE(isEqualMat(expected, actual));
}

