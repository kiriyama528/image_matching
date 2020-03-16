#include "pch.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

/*
#include "distance_transform_image.h"
#include "define.h"
#include "image_process.h"
*/

#include "contour_tracking.h"

class UnitTestDistanceTransformImage : public ::testing::Test {
protected:
	// エッジ画像として使う場合、1以外は0に置き換えて利用する
	// [0]channel目に書き込む
	unsigned char v[7*7] = {
		0, 0, 0, 0, 0, 0, 0,
		0, 1, 1, 1, 1, 1, 0,
		1, 2, 2, 2, 2, 1, 0,
		0, 1, 2, 3, 2, 1, 0,
		0, 1, 2, 2, 2, 1, 0,
		0, 0, 1, 1, 1, 0, 0,
		0, 0, 0, 0, 0, 0, 0
	};

	void toEdge(cv::Mat &dst, const cv::Mat src) {
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


TEST_F(UnitTestDistanceTransformImage, rasterScanForFirstValid) {
	cv::Mat edge;
	toEdge(edge, v, 7, 7);

	// 座標(0,0)から
	contourTracking ct;
	int act_r, act_c;
	ct.rasterScanForFirstValid(act_r, act_c, edge, 0, 0);

	int exp_r = 1, exp_c = 1;
	EXPECT_EQ(exp_r, act_r);
	EXPECT_EQ(exp_c, act_c);

	// 座標を途中から指定して
	ct.rasterScanForFirstValid(act_r, act_c, edge, 5, 1);

	exp_r = 5, exp_c = 2;
	EXPECT_EQ(exp_r, act_r);
	EXPECT_EQ(exp_c, act_c);
}


TEST_F(UnitTestDistanceTransformImage, recoursiveContourTracking_8) {
	const int rows = 5, cols = 5;
	unsigned char img[rows*cols] = {
		0, 0, 0, 0, 0,
		0, 1, 1, 1, 0,
		0, 1, 0, 1, 0,
		0, 1, 1, 1, 0,
		0, 0, 0, 0, 0
	};

	cv::Mat edge;
	toEdge(edge, img, rows, cols);
	cv::Mat process = cv::Mat::zeros(rows, cols, CV_8UC1);  // 追跡途中データを格納する行列
	int s_r = 1, s_c = 1;  // 0でない画素の座標。スタート地点となる。画像を変更したら、こちらも変更すること
	contourTracking ct;
	ct.recorsiveContourTracking(edge, process, s_r, s_c, contourTracking::LEFT, true);

	cv::Mat expected = cv::Mat::zeros(rows, cols, CV_8UC1);
	memcpy(expected.data, img, rows*cols);

	EXPECT_TRUE(isEqualMat(process, expected));
}
