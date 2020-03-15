#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "define.h"

using namespace cv;

/**
 * @brief imgname�Ŏw�肵���摜���J�� mat�Ɋi�[����
 * @param imgname <IN> �摜�p�X
 * @param mat <OUT> �摜�i�[��
 * @return (true) �摜�ǂݍ��ݐ��� / �摜�p�X���Ԉ���Ă��� (false)
 **/
bool readImage(const char * imgname, Mat & mat);

/**
 * @brief �摜(img)�̏����擾����
 * @param img <IN> �Ώۂ̉摜
 * @param rows <OUT> �s��
 * @param cols <OUT> ��
 * @param rows <OUT> �`�����l����
 * @return (true) �擾���� / �擾���s (false)
 **/
bool getImgInfo(const Mat & img, UTYPE * rows, UTYPE * cols, UTYPE * channels);

