#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>


class contourTracking {
public:
	// 8bit(uchar)�ŊǗ��ł���悤�Ȍ`��
	typedef enum {
		DEFAULT    = 0b00000000,
		UP_LEFT    = 0b00000001,
		UP         = 0b00000010,
		UP_RIGHT   = 0b00000100,
		LEFT       = 0b00001000,
		RIGHT      = 0b00010000,
		DOWN_LEFT  = 0b00100000,
		DOWN       = 0b01000000,
		DOWN_RIGHT = 0b10000000
	} DIRECTION;

	contourTracking();
	~contourTracking();

	/**
	 * @brief �֊s�ǐՌ��ʉ摜���G�b�W�摜�ɕϊ�����
	 * @param dst_edge <OUT> �o�͂������G�b�W�摜�i�[��
	 * @param src_tracking <IN> �֊s�ǐՌ��ʉ摜
	 **/
	static void trackingResultToEdge(cv::Mat & dst_edge, const cv::Mat & src_tracking);
	
	/**
	 * @brief �^����ꂽ����(d)���t�����ɂ���
	 * @param d <IN> �t�ɂ���������
	 * @return �t��������
	 **/
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

	/**
	 * @brief ���ډ�f(r,c)���������(to)�̉�f�l���擾����
	 * @param bi_img <IN> �T�������l�摜(CV_8UC1)
	 * @param r <IN> bi_img��row���W�B0~
	 * @param c <IN> bi_img��col���W�B0~
	 * @param to <IN> ������B�i��j���ډ�f�̍��̉�f�l���擾�������Ȃ�ALEFT
	 * @return ������̉�f�l
	 **/
	unsigned char getPixByDirection(const cv::Mat & bi_img, int r, int c, DIRECTION to);

	/**
	 * @brief ��f���͂ɂ��� 0�łȂ���f�l������f��������B1��������I���B�i���������玞�v���ɒT��
	 * @param dst_to <OUT> ���ʊi�[��
	 * @param bi_img <IN> �T�������l�摜(CV_8UC1)
	 * @param r <IN> bi_img��row���W�B0~
	 * @param c <IN> bi_img��col���W�B0~
	 * @param from <IN> �i�������B�i��j������Ȃ� LEFT
	 * @param is_8_neighborhood <IN> (true) 8�ߖT�T�� / 4�ߖT�T�� (false)
	 * @return (true) ���݂���(dst_to�֊i�[) / ���݂��Ȃ����� (false)
	 **/
	bool searchValidPixAround(DIRECTION * dst_to, const cv::Mat & bi_img, int r, int c, DIRECTION from, bool is_8_neighborhood);

	/**
	 * @brief �ċA�I��1��f���ǐՂ��s�����������i�R�A�j�Bcalled by run()�B
	 *        �O�����璼�ڌĂяo�����Ƃ͂Ȃ��\��B
	 *        TODO protected �Ɉړ��\��
	 * @param bi_img <IN> �T�������l�摜(CV_8UC1)
	 * @param process <OUT> �T���̓r�����ʂ��i�[����摜�B���O��CV_8UC1�ŏ���������Ă��邱�ƁB
	 * @param r <IN> bi_img��row���W�B0~
	 * @param c <IN> bi_img��col���W�B0~
	 * @param from <IN> �i�������B�i��j������Ȃ� LEFT
	 * @param is_8_neighborhood <IN> (true) 8�ߖT�T�� / 4�ߖT�T�� (false)
	 **/
	void recursiveContourTracking(const cv::Mat & bi_img, cv::Mat & process, int r, int c, DIRECTION from, bool is_8_neighborhood);

	/**
	 * @brief �֊s�ǐՁB���v���B���̊֐����Ăяo���̂���{
	 * @param dst <OUT> �ǐՌ��ʉ摜
	 * @param bi_img <IN> ��l�摜(CV_8UC1)
	 * @param start_row <IN> �T���J�nrow���W (0~
	 * @param start_col <IN> �T���J�ncol���W (0~
	 * @param is_8_neighborhood 8�ߖT�T�����ǂ���
	 * @return (true) ���� / ���s (false)
	 **/
	bool run(cv::Mat & dst, const cv::Mat & bi_img, int start_row, int start_col, bool is_8_neighborhood = true);

};

/**
 * @brief ���W(r, c) ���L���͈͓���
 * @param r <IN> row���W�B0~
 * @param c <IN> col���W�B0~
 * @param img_rows <IN> �Ώۉ摜��row��
 * @param img_cols <IN> �Ώۉ摜��col��
 * @return (true) �L�� / �͈͊O (false)
 **/
bool validCoord(int r, int c, int img_rows, int img_cols);
