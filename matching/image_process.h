#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "define.h"

using namespace cv;

/**
 * @brief imgnameで指定した画像を開き matに格納する
 * @param imgname <IN> 画像パス
 * @param mat <OUT> 画像格納先
 * @return (true) 画像読み込み成功 / 画像パスが間違っている (false)
 **/
bool readImage(const char * imgname, Mat & mat);

/**
 * @brief 画像(img)の情報を取得する
 * @param img <IN> 対象の画像
 * @param rows <OUT> 行数
 * @param cols <OUT> 列数
 * @param rows <OUT> チャンネル数
 * @return (true) 取得成功 / 取得失敗 (false)
 **/
bool getImgInfo(const Mat & img, UTYPE * rows, UTYPE * cols, UTYPE * channels);

/**
 * @brief 画素値へのアクセス取得。setもgetもできる
 * @param img <IN> 画像行列
 * @param row <IN> 行番号 0~
 * @param col <IN> 列番号 0~
 * @param channel <IN> チャネル番号 0~
 * @return アクセス先。代入左辺で使用すれば[set]として、右辺で使用すれば[get]として利用できる
 **/
unsigned char & at(Mat & img, UTYPE row, UTYPE col, UTYPE channel);
