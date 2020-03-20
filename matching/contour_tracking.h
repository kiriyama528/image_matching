#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>


class contourTracking {
public:
	// 0bXXXXX �`���ŏ���
	typedef enum {
		DEFAULT = 0,
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

	// �ǐՌ��ʉ摜���G�b�W�摜�ɕϊ�����
	static void trackingResultToEdge(cv::Mat & dst_edge, const cv::Mat & src_tracking);
	
	// �^�����Ă�����(d)���t�����ɂ���
	DIRECTION reverseDirection(DIRECTION d);
	
	/**
	 * @brief �w�肳�ꂽ���W(start_row, start_col)���烉�X�^�X�L�������J�n���A�ŏ���0�ȊO�̒l������f��������
	 * @param dst_r
	 * @param dst_c
	 * @return (true) ���݂��� / ���݂��Ȃ����� (false)
	 **/
	bool rasterScanForFirstValid(int & dst_r, int & dst_c, const cv::Mat & bi_img, int start_row, int start_col);

	/**
	 * @brief ���ݍ��W(r,c)����w�����������̍��W(dst_r, dst_c)���擾����
	 * @param img <IN> �Ώۉ摜
	 * @param dst_r <OUT> �ړ����row
	 * @param dst_c <OUT> �ړ����col
	 * @param r <IN> ���ݍ��W�� row
	 * @param c <IN> ���ݍ��W�� col
	 * @param to <IN> �s����������
	 * @return (true) ���W���擾���� / �摜�͈̔͊O������ (false)
	 **/
	bool directionToRC(const cv::Mat &img, int * dst_r, int * dst_c, int r, int c, DIRECTION to);

	// ������̉�f�l���擾����
	unsigned char getPixByDirection(const cv::Mat & bi_img, int r, int c, DIRECTION to);

	// ���͂� 0 �łȂ���f��������
	bool searchValidPixAround(DIRECTION * dst_to, const cv::Mat & bi_img, int r, int c, DIRECTION from, bool is_8_neighborhood);

	/**
	 * @brief �ċA�I�ɒǐՂ��s���R�A�����Bcalled by run()�B�O�����璼�ڌĂяo�����Ƃ͂Ȃ��\��
	 * TODO protected �Ɉړ��\��
	 *
	 *
	 **/
	void recorsiveContourTracking(const cv::Mat & bi_img, cv::Mat & process, int r, int c, DIRECTION from, bool is_8_neighborhood);

	/**
	 * @brief �֊s�ǐՁB���v���B���̊֐����Ăяo���̂���{
	 * @param dst <OUT> �ǐՌ��ʉ摜
	 * @param bi_img <IN> ��l�摜
	 * @param start_row <IN> �T���J�nrow���W (0~
	 * @param start_col <IN> �T���J�ncol���W (0~
	 * @param is_8_neighborhood 8�ߖT�T�����ǂ���
	 **/
	void run(cv::Mat & dst, const cv::Mat & bi_img, int start_row, int start_col, bool is_8_neighborhood = true);

};

/**
 * @brief ���W(r, c) ���L���͈͓���
 * @return (true) �L�� / �͈͊O (false)
 **/
bool validCoord(int r, int c, int img_rows, int img_cols);
