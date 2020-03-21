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
	// �G�b�W�摜�Ƃ��Ďg���ꍇ�A1�ȊO��0�ɒu�������ė��p����
	// [0]channel�ڂɏ�������
	unsigned char v[7*7] = {
		0, 0, 0, 0, 0, 0, 0,
		0, 1, 1, 1, 1, 1, 0,
		1, 2, 2, 2, 2, 1, 0,
		0, 1, 2, 3, 2, 1, 0,
		0, 1, 2, 2, 2, 1, 0,
		0, 0, 1, 1, 1, 0, 0,
		0, 0, 0, 0, 0, 0, 0
	};

	void toEdge(cv::Mat &dst, const cv::Mat &src) {
		// �G�b�W�摜��p�ӂ���
		int rows = src.rows;
		int cols = src.cols;
		dst = cv::Mat::zeros(rows, cols, CV_8UC1);
		unsigned char *dst_data = dst.data;
		unsigned char *src_data = src.data;
		for (int r = 0; r < rows; r++) {
			for (int c = 0; c < rows; c++) {
				int idx = r * cols + c; // channel����1channel�Ɍ��ߑł�
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

	// ���W(0,0)����
	contourTracking ct;
	int act_r, act_c;
	ct.rasterScanForFirstValid(act_r, act_c, edge, 0, 0);

	int exp_r = 1, exp_c = 1;
	EXPECT_EQ(exp_r, act_r);
	EXPECT_EQ(exp_c, act_c);

	// ���W��r������w�肵��
	ct.rasterScanForFirstValid(act_r, act_c, edge, 5, 1);

	exp_r = 5, exp_c = 2;
	EXPECT_EQ(exp_r, act_r);
	EXPECT_EQ(exp_c, act_c);
}


TEST_F(UnitTestDistanceTransformImage, recursiveContourTracking_8) {
	const int rows = 5, cols = 5;
	unsigned char img[rows*cols] = {
		0, 0, 0, 0, 0,
		0, 1, 1, 1, 0,
		0, 1, 0, 1, 0,
		0, 1, 1, 1, 0,
		0, 0, 0, 0, 0
	};

	unsigned char exp_data[rows*cols] = {
		0,  0,  0,  0,  0,
		0, 64,  8,  8,  0,
		0, 64,  0,  2,  0,
		0, 16, 16,  2,  0,
		0,  0,  0,  0,  0
	};

	cv::Mat edge;
	toEdge(edge, img, rows, cols);
	cv::Mat process = cv::Mat::zeros(rows, cols, CV_8UC1);  // �ǐՓr���f�[�^���i�[����s��
	int s_r = 1, s_c = 1;  // 0�łȂ���f�̍��W�B�X�^�[�g�n�_�ƂȂ�B�摜��ύX������A��������ύX���邱��
	contourTracking ct;
	ct.recursiveContourTracking(edge, process, s_r, s_c, contourTracking::LEFT, true);

	cv::Mat expected = cv::Mat::zeros(rows, cols, CV_8UC1);
	memcpy(expected.data, exp_data, rows*cols);

	EXPECT_TRUE(isEqualMat(process, expected));
}


TEST_F(UnitTestDistanceTransformImage, reverseDirection) {
	const int n_DIRECTION = 9;
	contourTracking::DIRECTION inp_expected[n_DIRECTION][2] = {
		{contourTracking::DEFAULT, contourTracking::DEFAULT},
		{contourTracking::UP_LEFT, contourTracking::DOWN_RIGHT},
		{contourTracking::UP, contourTracking::DOWN},
		{contourTracking::UP_RIGHT, contourTracking::DOWN_LEFT},
		{contourTracking::LEFT, contourTracking::RIGHT},
		{contourTracking::RIGHT, contourTracking::LEFT},
		{contourTracking::DOWN_LEFT, contourTracking::UP_RIGHT},
		{contourTracking::DOWN, contourTracking::UP},
		{contourTracking::DOWN_RIGHT, contourTracking::UP_LEFT}
	};

	contourTracking ct;
	for (int i = 0; i < n_DIRECTION; i++) {
		contourTracking::DIRECTION actual =ct.reverseDirection(inp_expected[i][0]);
		EXPECT_EQ(inp_expected[i][1], actual);
	}
}

// �{���͂��̃e�X�g�t�B�N�X�`���ł͂Ȃ����A�ȒP�Ȃ̂ŗ��p
TEST_F(UnitTestDistanceTransformImage, validCoord) {
	EXPECT_EQ(validCoord(-1, -1, 10, 20), false);
	EXPECT_EQ(validCoord(-1, 0, 10, 20), false);
	EXPECT_EQ(validCoord(0, -1, 10, 20), false);
	EXPECT_EQ(validCoord(0, 0, 10, 20), true);
	EXPECT_EQ(validCoord(9, 19, 10, 20), true);
	EXPECT_EQ(validCoord(10, 19, 10, 20), false);
	EXPECT_EQ(validCoord(9, 20, 10, 20), false);
	EXPECT_EQ(validCoord(10, 20, 10, 20), false);
}


TEST_F(UnitTestDistanceTransformImage, directionToRC) {
	cv::Mat img(7, 7, CV_8UC1);
	memcpy(img.data, v, 7 * 7);

	contourTracking ct;
	int target_r, target_c;
	int act_r, act_c;
	int exp_r, exp_c;

	// ����[���W(0,0)�ɑ΂���
	target_r = 0, target_c = 0;
	bool act_ret = ct.directionToRC(img, &act_r, &act_c, target_r, target_c,
		contourTracking::DOWN_LEFT);
	EXPECT_EQ(act_ret, false);

	act_ret = ct.directionToRC(img, &act_r, &act_c, target_r, target_c,
		contourTracking::LEFT);
	EXPECT_EQ(act_ret, false);

	act_ret = ct.directionToRC(img, &act_r, &act_c, target_r, target_c,
		contourTracking::UP_LEFT);
	EXPECT_EQ(act_ret, false);

	act_ret = ct.directionToRC(img, &act_r, &act_c, target_r, target_c,
		contourTracking::UP);
	EXPECT_EQ(act_ret, false);

	act_ret = ct.directionToRC(img, &act_r, &act_c, target_r, target_c,
		contourTracking::UP_RIGHT);
	EXPECT_EQ(act_ret, false);

	exp_r = target_r, exp_c = target_c + 1;
	act_ret = ct.directionToRC(img, &act_r, &act_c, target_r, target_c,
		contourTracking::RIGHT);
	EXPECT_EQ(act_ret, true);
	EXPECT_EQ(exp_r, act_r);
	EXPECT_EQ(exp_c, act_c);

	exp_r = 1, exp_c = 1;
	exp_r = target_r + 1, exp_c = target_c + 1;
	act_ret = ct.directionToRC(img, &act_r, &act_c, target_r, target_c,
		contourTracking::DOWN_RIGHT);
	EXPECT_EQ(act_ret, true);
	EXPECT_EQ(exp_r, act_r);
	EXPECT_EQ(exp_c, act_c);

	exp_r = target_r + 1, exp_c = target_c;
	act_ret = ct.directionToRC(img, &act_r, &act_c, target_r, target_c,
		contourTracking::DOWN);
	EXPECT_EQ(act_ret, true);
	EXPECT_EQ(exp_r, act_r);
	EXPECT_EQ(exp_c, act_c);

	// �E���[���W(6,6)�ɑ΂���
	target_r = 6, target_c = 6;
	act_ret = ct.directionToRC(img, &act_r, &act_c, target_r, target_c,
		contourTracking::UP_RIGHT);
	EXPECT_EQ(act_ret, false);

	act_ret = ct.directionToRC(img, &act_r, &act_c, target_r, target_c,
		contourTracking::RIGHT);
	EXPECT_EQ(act_ret, false);

	act_ret = ct.directionToRC(img, &act_r, &act_c, target_r, target_c,
		contourTracking::DOWN_RIGHT);
	EXPECT_EQ(act_ret, false);

	act_ret = ct.directionToRC(img, &act_r, &act_c, target_r, target_c,
		contourTracking::DOWN);
	EXPECT_EQ(act_ret, false);

	act_ret = ct.directionToRC(img, &act_r, &act_c, target_r, target_c,
		contourTracking::DOWN_LEFT);
	EXPECT_EQ(act_ret, false);

	exp_r = target_r, exp_c = target_c -1;
	act_ret = ct.directionToRC(img, &act_r, &act_c, target_r, target_c,
		contourTracking::LEFT);
	EXPECT_EQ(act_ret, true);
	EXPECT_EQ(exp_r, act_r);
	EXPECT_EQ(exp_c, act_c);

	exp_r = target_r - 1, exp_c = target_c - 1;
	act_ret = ct.directionToRC(img, &act_r, &act_c, target_r, target_c,
		contourTracking::UP_LEFT);
	EXPECT_EQ(act_ret, true);
	EXPECT_EQ(exp_r, act_r);
	EXPECT_EQ(exp_c, act_c);

	exp_r = target_r - 1, exp_c = target_c;
	act_ret = ct.directionToRC(img, &act_r, &act_c, target_r, target_c,
		contourTracking::UP);
	EXPECT_EQ(act_ret, true);
	EXPECT_EQ(exp_r, act_r);
	EXPECT_EQ(exp_c, act_c);
}


TEST_F(UnitTestDistanceTransformImage, trackingResultToEdge) {
	const int rows = 5, cols = 5;
	unsigned char src_data[rows*cols] = {
		4,   0,   0,   0,  255,
		0,   16,  32,  64, 0,
		0,   8,   128, 1,  0,
		0,   2,   1,   1,  0,
		128, 0,   0,   0,  255
	};

	unsigned char exp_data[rows*cols] = {
		1,   0,   0,   0,  1,
		0,   1,   1,   1,  0,
		0,   1,   1,   1,  0,
		0,   1,   1,   1,  0,
		1,   0,   0,   0,  1
	};

	cv::Mat tracking(rows, cols, CV_8UC1);
	memcpy(tracking.data, src_data, rows * cols);
	
	cv::Mat actual;
	contourTracking ct;
	ct.trackingResultToEdge(actual, tracking);

	cv::Mat expected(rows, cols, CV_8UC1);
	memcpy(expected.data, exp_data, rows * cols);

	EXPECT_TRUE(isEqualMat(actual, expected));
}


// �֊s�ǐ�(contourTracking�̌Ăяo���R�A)
TEST_F(UnitTestDistanceTransformImage, contourTrackingRun) {
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
	cv::Mat actual = cv::Mat::zeros(rows, cols, CV_8UC1);  // �ǐՓr���f�[�^���i�[����s��
	cv::Mat before = actual;
	int s_r = 0, s_c = 0;  // �X�^�[�g�n�_�B�ǂ��ł������B
	contourTracking ct;
	ct.run(actual, edge, s_r, s_c, true);
	
	// run()�֐��̃G���[�����Ɉ����������ĂȂ����m�F
	EXPECT_FALSE(isEqualMat(actual, before));

	cv::Mat expected = cv::Mat::zeros(rows, cols, CV_8UC1);
	memcpy(expected.data, img, rows*cols);
	
	EXPECT_TRUE(isEqualMat(actual, expected));
}


TEST_F(UnitTestDistanceTransformImage, searchValidPixAround) {
	const int rows = 5, cols = 5;
	unsigned char img[rows*cols] = {
		0, 0, 0, 0, 0,
		0, 0, 1, 0, 0,
		0, 1, 1, 1, 0,
		0, 0, 1, 1, 0,
		0, 0, 0, 0, 0
	};

	cv::Mat edge;
	toEdge(edge, img, rows, cols);

	contourTracking ct;
	// �A���T����͂��ăe�X�g
	contourTracking::DIRECTION actual;
	contourTracking::DIRECTION from = contourTracking::LEFT;
	bool act_ret = ct.searchValidPixAround(&actual, edge, 2, 1
		, from, true);
	EXPECT_TRUE(act_ret);
	contourTracking::DIRECTION expected = contourTracking::UP_RIGHT;
	EXPECT_EQ(actual, expected);

	from = ct.reverseDirection(expected);
	act_ret = ct.searchValidPixAround(&actual, edge, 1, 2
		, from, true);
	EXPECT_TRUE(act_ret);
	expected = contourTracking::DOWN_RIGHT;
	EXPECT_EQ(actual, expected);

	from = ct.reverseDirection(expected);
	act_ret = ct.searchValidPixAround(&actual, edge, 2, 3
		, from, true);
	EXPECT_TRUE(act_ret);
	expected = contourTracking::DOWN;
	EXPECT_EQ(actual, expected);

	from = ct.reverseDirection(expected);
	act_ret = ct.searchValidPixAround(&actual, edge, 3, 3
		, from, true);
	EXPECT_TRUE(act_ret);
	expected = contourTracking::LEFT;
	EXPECT_EQ(actual, expected);

	from = ct.reverseDirection(expected);
	act_ret = ct.searchValidPixAround(&actual, edge, 3, 2
		, from, true);
	EXPECT_TRUE(act_ret);
	expected = contourTracking::UP_LEFT;
	EXPECT_EQ(actual, expected);

	from = ct.reverseDirection(expected);
	act_ret = ct.searchValidPixAround(&actual, edge, 2, 1
		, from, true);
	EXPECT_TRUE(act_ret);
	expected = contourTracking::UP_RIGHT;
	EXPECT_EQ(actual, expected);

	// �P�̃`�F�b�N�B���W(2, 2)�ɑ΂��đS��������i��
	contourTracking::DIRECTION from_exp[8][2] = {
		contourTracking::LEFT,       contourTracking::UP,
		contourTracking::UP_LEFT,    contourTracking::UP,
		contourTracking::UP,         contourTracking::RIGHT,
		contourTracking::UP_RIGHT,   contourTracking::RIGHT,
		contourTracking::RIGHT,      contourTracking::DOWN_RIGHT,
		contourTracking::DOWN_RIGHT, contourTracking::DOWN,
		contourTracking::DOWN,       contourTracking::LEFT,
		contourTracking::DOWN_LEFT,  contourTracking::LEFT,
	};

	for (int i = 0; i < 8; i++) {
		act_ret = ct.searchValidPixAround(&actual, edge, 2, 2
			, from_exp[i][0], true);
		EXPECT_TRUE(act_ret);
		EXPECT_EQ(actual, from_exp[i][1]);
	}
	
	// �G���[�`�F�b�N�B���͂ɉ����L����f���Ȃ��ꍇ
	// ���W(0,4)
	from = contourTracking::LEFT;
	act_ret = ct.searchValidPixAround(&actual, edge, 0, 4
		, from, true);
	EXPECT_FALSE(act_ret);
}

