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
		
	// �G�b�W�摜�Ƃ��Ďg���ꍇ�A1�ȊO��0�ɒu�������ė��p����
	// [0]channel�ڂɏ�������
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
	// �G�b�W�摜��p�ӂ���
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < rows; c++) {
			int idx = r * cols + c; // channel����1channel�Ɍ��ߑł�
			if (v[idx] == 1) {
				data[idx] = v[idx];
			}
		}
	}
	
	cv::Mat actual;
	makeDistanceTransformImage(actual, src); // making ���̓X�^�u���

	cv::Mat expected(rows, cols, CV_8UC1);
	memcpy(expected.data, v, rows*cols);

	EXPECT_TRUE(isEqualMat(expected, actual));
}

TEST(UnitTestDistanceTransformImage, rasterScanForFirstValid) {
	const int rows = 6, cols = 6;

	// �G�b�W�摜�Ƃ��Ďg���ꍇ�A1�ȊO��0�ɒu�������ė��p����
	// [0]channel�ڂɏ�������
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
	// �G�b�W�摜��p�ӂ���
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < rows; c++) {
			int idx = r * cols + c; // channel����1channel�Ɍ��ߑł�
			if (v[idx] == 1) {
				data[idx] = v[idx];
			}
		}
	}

	// ���W(0,0)����
	int act_r, act_c;
	rasterScanForFirstValid(act_r, act_c, src, 0, 0);

	int exp_r = 0, exp_c = 1;
	EXPECT_EQ(exp_r, act_r);
	EXPECT_EQ(exp_c, act_c);

	// ���W��r������w�肵��
	rasterScanForFirstValid(act_r, act_c, src, 4, 1);

	exp_r = 4, exp_c = 2;
	EXPECT_EQ(exp_r, act_r);
	EXPECT_EQ(exp_c, act_c);
}
