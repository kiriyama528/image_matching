#include "pch.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

/*
#include "distance_transform_image.h"
#include "define.h"
#include "image_process.h"
*/

#include "contour_tracking.h"

TEST(UnitTestDistanceTransformImage, rasterScanForFirstValid) {
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

	contourTracking ct;

	// 座標(0,0)から
	int act_r, act_c;
	ct.rasterScanForFirstValid(act_r, act_c, src, 0, 0);

	int exp_r = 0, exp_c = 1;
	EXPECT_EQ(exp_r, act_r);
	EXPECT_EQ(exp_c, act_c);

	// 座標を途中から指定して
	ct.rasterScanForFirstValid(act_r, act_c, src, 4, 1);

	exp_r = 4, exp_c = 2;
	EXPECT_EQ(exp_r, act_r);
	EXPECT_EQ(exp_c, act_c);
}
