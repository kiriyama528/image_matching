#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>


class contourTracking {
public:
	typedef enum {
		UP_LEFT = 1,
		UP = 2,
		UP_RIGHT = 4,
		LEFT = 8,
		RIGHT = 16,
		DOWN_LEFT = 32,
		DOWN = 64,
		DOWN_RIGHT = 128
	} DIRECTION;

	contourTracking();
	~contourTracking();
	
	/**
	 * @brief �w�肳�ꂽ���W(start_row, start_col)���烉�X�^�X�L�������J�n���A�ŏ���0�ȊO�̒l������f��������
	 * @param dst_r
	 * @param dst_c
	 * @return (true) ���݂��� / ���݂��Ȃ����� (false)
	 **/
	bool rasterScanForFirstValid(int & dst_r, int & dst_c, const cv::Mat & bi_img, int start_row, int start_col);

	/**
	 * @brief �֊s�ǐՁB���v���
	 * @param dst <OUT> �ǐՌ��ʉ摜
	 * @param bi_img <IN> ��l�摜
	 * @param start_row <IN> �T���J�nrow���W (0~
	 * @param start_col <IN> �T���J�ncol���W (0~
	 * @param is_8_neighborhood 8�ߖT�T�����ǂ���
	 **/
	void run(cv::Mat & dst, const cv::Mat & bi_img, int start_row, int start_col, bool is_8_neighborhood);

};