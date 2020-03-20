#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>


class contourTracking {
public:
	// 0bXXXXX 形式で書け
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

	// 追跡結果画像をエッジ画像に変換する
	static void trackingResultToEdge(cv::Mat & dst_edge, const cv::Mat & src_tracking);
	
	// 与えられてた方向(d)を逆向きにする
	DIRECTION reverseDirection(DIRECTION d);
	
	/**
	 * @brief 指定された座標(start_row, start_col)からラスタスキャンを開始し、最初に0以外の値を持つ画素を見つける
	 * @param dst_r
	 * @param dst_c
	 * @return (true) 存在した / 存在しなかった (false)
	 **/
	bool rasterScanForFirstValid(int & dst_r, int & dst_c, const cv::Mat & bi_img, int start_row, int start_col);

	/**
	 * @brief 現在座標(r,c)から指示した方向の座標(dst_r, dst_c)を取得する
	 * @param img <IN> 対象画像
	 * @param dst_r <OUT> 移動先のrow
	 * @param dst_c <OUT> 移動先のcol
	 * @param r <IN> 現在座標の row
	 * @param c <IN> 現在座標の col
	 * @param to <IN> 行きたい方向
	 * @return (true) 座標を取得した / 画像の範囲外だった (false)
	 **/
	bool directionToRC(const cv::Mat &img, int * dst_r, int * dst_c, int r, int c, DIRECTION to);

	// 方向先の画素値を取得する
	unsigned char getPixByDirection(const cv::Mat & bi_img, int r, int c, DIRECTION to);

	// 周囲の 0 でない画素を見つける
	bool searchValidPixAround(DIRECTION * dst_to, const cv::Mat & bi_img, int r, int c, DIRECTION from, bool is_8_neighborhood);

	/**
	 * @brief 再帰的に追跡を行うコア部分。called by run()。外部から直接呼び出すことはない予定
	 * TODO protected に移動予定
	 *
	 *
	 **/
	void recorsiveContourTracking(const cv::Mat & bi_img, cv::Mat & process, int r, int c, DIRECTION from, bool is_8_neighborhood);

	/**
	 * @brief 輪郭追跡。時計回り。この関数を呼び出すのが基本
	 * @param dst <OUT> 追跡結果画像
	 * @param bi_img <IN> 二値画像
	 * @param start_row <IN> 探索開始row座標 (0~
	 * @param start_col <IN> 探索開始col座標 (0~
	 * @param is_8_neighborhood 8近傍探索かどうか
	 **/
	void run(cv::Mat & dst, const cv::Mat & bi_img, int start_row, int start_col, bool is_8_neighborhood = true);

};

/**
 * @brief 座標(r, c) が有効範囲内か
 * @return (true) 有効 / 範囲外 (false)
 **/
bool validCoord(int r, int c, int img_rows, int img_cols);
