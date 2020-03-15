#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "image_process.h"
#include "define.h"
#include "distance_transform_image.h"





/**
 * @brief 指定された座標(start_row, start_col)からラスタスキャンを開始し、最初に0以外の値を持つ画素を見つける
 * @param dst_r
 * @param dst_c
 * @return (true) 存在した / 存在しなかった (false)
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
 * @brief 輪郭追跡。時計回り
 * @param dst <OUT> 追跡結果画像
 * @param bi_img <IN> 二値画像
 * @param start_row <IN> 探索開始row座標 (0~
 * @param start_col <IN> 探索開始col座標 (0~
 * @param is_8_neighborhood 8近傍探索かどうか
 **/
void contourTracking(cv::Mat &dst, const cv::Mat &bi_img, int start_row, int start_col, bool is_8_neighborhood = true) {
	UTYPE rows, cols, channels;
	if (getImgInfo(bi_img, &rows, &cols, &channels) == false) {
		fprintf(stderr, " > ERROR: 画像が読み込まれていません\n");
		return;
	}

	// 最初の画素を見つけて(r,c)に格納する
	int r = 0, c = 0;
	if (rasterScanForFirstValid(r, c, bi_img, 0, 0) == false) {
		fprintf(stderr, " > ERROR: 画素値が0出ない画素が存在しませんでした。\n");
		return;
	}


	//// indexをひとつ前に戻したrとcを計算する
	//// indexからrとcを計算する関数を作る。


	// making
	// 侵入方向を示しつつ、周囲探索。
	// 4近傍と8近傍で周囲の定義が違う
	//////serchAroundValidPix();

}


void makeDistanceTransformImage(cv::Mat &dst, const cv::Mat &src) {
	UTYPE rows, cols, channels;
	getImgInfo(src, &rows, &cols, &channels);
	dst = cv::Mat::zeros(rows, cols, CV_8UC1);

	/*
	// TODO 実装を作れ。今はGREEN
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