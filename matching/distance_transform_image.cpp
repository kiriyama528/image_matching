#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "image_process.h"
#include "define.h"
#include "distance_transform_image.h"





/**
 * @brief �w�肳�ꂽ���W(start_row, start_col)���烉�X�^�X�L�������J�n���A�ŏ���0�ȊO�̒l������f��������
 * @param dst_r
 * @param dst_c
 * @return (true) ���݂��� / ���݂��Ȃ����� (false)
 **/
bool rasterScanForFirstValid(int & dst_r, int & dst_c, const cv::Mat & bi_img, int start_row, int start_col)
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

/**
 * @brief �֊s�ǐՁB���v���
 * @param dst <OUT> �ǐՌ��ʉ摜
 * @param bi_img <IN> ��l�摜
 * @param start_row <IN> �T���J�nrow���W (0~
 * @param start_col <IN> �T���J�ncol���W (0~
 * @param is_8_neighborhood 8�ߖT�T�����ǂ���
 **/
void contourTracking(cv::Mat &dst, const cv::Mat &bi_img, int start_row, int start_col, bool is_8_neighborhood = true) {
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


	//// index���ЂƂO�ɖ߂���r��c���v�Z����
	//// index����r��c���v�Z����֐������B


	// making
	// �N�������������A���͒T���B
	// 4�ߖT��8�ߖT�Ŏ��͂̒�`���Ⴄ
	//////serchAroundValidPix();

}


void makeDistanceTransformImage(cv::Mat &dst, const cv::Mat &src) {
	UTYPE rows, cols, channels;
	getImgInfo(src, &rows, &cols, &channels);
	dst = cv::Mat::zeros(rows, cols, CV_8UC1);

	/*
	// TODO ���������B����GREEN
	unsigned char v[36] = {
		0, 1, 1, 1, 1, 1,
		1, 2, 2, 2, 2, 1,
		0, 1, 2, 3, 2, 1,
		0, 1, 2, 2, 2, 1,
		0, 0, 1, 1, 1, 0,
		0, 0, 0, 0, 0, 0
	};
	memcpy(dst.data, v, 36);
	*/

}