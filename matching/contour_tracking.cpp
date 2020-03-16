
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "contour_tracking.h"
#include "define.h"
#include "image_process.h"

contourTracking::contourTracking()
{
}

contourTracking::~contourTracking()
{
}

bool contourTracking::rasterScanForFirstValid(int & dst_r, int & dst_c, const cv::Mat & bi_img, int start_row, int start_col)
{
	for (dst_r = start_row; dst_r < bi_img.rows; dst_r++) {
		for (dst_c = start_col; dst_c < bi_img.cols; dst_c++) {
			if (at(bi_img, dst_r, dst_c, 0) != 0) {
				return true;
			}
		}
	}

	return false;
}

bool validCoord(int r, int c, int img_rows, int img_cols) {
	if (r < 0 && img_rows <= r) {
		return false;
	}
	if (c < 0 && img_cols <= r) {
		return false;
	}

	return true;
}

// ������̉�f�l���擾����
unsigned char contourTracking::getPixByDirection(const cv::Mat & bi_img, int r, int c, DIRECTION to) {
	int move_r=0, move_c=0;
	switch (to) {
	case UP_LEFT:
		move_r = -1;
		move_c = -1;
		break;
	case UP:
		move_r = -1;
		move_c = 0;
		break;
	case UP_RIGHT:
		move_r = -1;
		move_c = 1;
		break;
	case LEFT:
		move_r = 0;
		move_c = -1;
		break;
	case RIGHT:
		move_r = 0;
		move_c = 1;
		break;
	case DOWN_LEFT:
		move_r = 1;
		move_c = -1;
		break;
	case DOWN:
		move_r = 1;
		move_c = 0;
		break;
	case DOWN_RIGHT:
		move_r = 1;
		move_c = 1;
		break;
	default:
		// do nothing
		break;
	}

	r = r + move_r;
	c = c + move_c;
	if (validCoord(r, c, bi_img.rows, bi_img.cols) == false) {
		// ERROR
		return 0; // fix me
	}

	return at(bi_img, r, c, 0);
}


// ���͂� 0 �o�Ȃ���f��������
// ���������I��
// �i���������玞�v���ɒT������
void contourTracking::searchValidPixAround(const cv::Mat &bi_img, int r, int c, DIRECTION from, bool is_8_neighborhood) {
	const DIRECTION around8_idx[] = { UP_LEFT, UP, UP_RIGHT, RIGHT, DOWN_RIGHT, DOWN, DOWN_LEFT, LEFT};
	const DIRECTION around4_idx[] = { UP, RIGHT, DOWN, LEFT };
	
	const DIRECTION *around_idx;
	int n_idx;
	if (is_8_neighborhood) {
		around_idx = around8_idx;
		n_idx = 8;
	}
	else {
		around_idx = around4_idx;
		n_idx = 4;
	}
	
	int start_idx = -1;
	for (int i = 0; i < n_idx; i++) {
		if (around_idx[i] == from) {
			start_idx = i;
			break;
		}
	}

	if (start_idx = -1) {
		// ERROR
		return; // fix me 
	}
	
	for (int i = 0; i < n_idx; i++) {
		// �����O��ɃA�N�Z�X
		int idx = (start_idx + i) % n_idx;
		unsigned char value = getPixByDirection(bi_img, r, c, around_idx[idx]);
	}

}


// �Ƃ肠����void�A8�ߖT���ߑł�
// TODO 4�ߖT�ɂ�����ł���悤�ɂ���
void contourTracking::recorsiveContourTracking(const cv::Mat & bi_img, cv::Mat & process, int r, int c, DIRECTION from, bool is_8_neighborhood) {
#if 0 // �{����
	serchValidPixAround(r, c, );

#else // ����GREEN�R�[�h
	unsigned char img[5*5] = {
		0, 0, 0, 0, 0,
		0, 1, 1, 1, 0,
		0, 1, 0, 1, 0,
		0, 1, 1, 1, 0,
		0, 0, 0, 0, 0
	};

	memcpy(process.data, img, 5 * 5);
	
#endif
}


void contourTracking::run(cv::Mat &dst, const cv::Mat &bi_img, int start_row, int start_col, bool is_8_neighborhood) {
	UTYPE rows, cols, channels;
	if (getImgInfo(bi_img, &rows, &cols, &channels) == false) {
		fprintf(stderr, " > ERROR: �摜���ǂݍ��܂�Ă��܂���\n");
		return;
	}

	// �ŏ��̉�f��������(r,c)�Ɋi�[����
	int r = 0, c = 0;
	if (rasterScanForFirstValid(r, c, bi_img, 0, 0) == false) {
		fprintf(stderr, " > ERROR: ��f�l��0�o�Ȃ���f�����݂��܂���ł����B\n");
		return;
	}


	// making
	// �N�������������A���͒T���B
	// 4�ߖT��8�ߖT�Ŏ��͂̒�`���Ⴄ
	
	cv::Mat process = cv::Mat::zeros(rows, cols, CV_8UC1);  // �ǐՓr���f�[�^���i�[����s��
	recorsiveContourTracking(bi_img, process, r, c, LEFT, is_8_neighborhood);
	
}