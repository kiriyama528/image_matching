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
	 * @brief 指定された座標(start_row, start_col)からラスタスキャンを開始し、最初に0以外の値を持つ画素を見つける
	 * @param dst_r
	 * @param dst_c
	 * @return (true) 存在した / 存在しなかった (false)
	 **/
	bool rasterScanForFirstValid(int & dst_r, int & dst_c, const cv::Mat & bi_img, int start_row, int start_col);

	/**
	 * @brief 輪郭追跡。時計回り
	 * @param dst <OUT> 追跡結果画像
	 * @param bi_img <IN> 二値画像
	 * @param start_row <IN> 探索開始row座標 (0~
	 * @param start_col <IN> 探索開始col座標 (0~
	 * @param is_8_neighborhood 8近傍探索かどうか
	 **/
	void run(cv::Mat & dst, const cv::Mat & bi_img, int start_row, int start_col, bool is_8_neighborhood);

};
