
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


// �Ƃ肠����void
// MEMO �Ƃ肠����8�ߖT�̌��ߑł��ō��
void contourTracking::recorsiveContourTracking(const cv::Mat & bi_img, cv::Mat & process, int r, int c, DIRECTION from, bool is_8_neighborhood) {

	//serchValidPixAround(r, c, );

	unsigned char img[5*5] = {
		0, 0, 0, 0, 0,
		0, 1, 1, 1, 0,
		0, 1, 0, 1, 0,
		0, 1, 1, 1, 0,
		0, 0, 0, 0, 0
	};

	memcpy(process.data, img, 5 * 5);
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