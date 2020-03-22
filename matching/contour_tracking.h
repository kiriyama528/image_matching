#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>


class contourTracking {
public:
	// 8bit(uchar)で管理できるような形式
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
	 * @brief 輪郭追跡結果画像をエッジ画像に変換する
	 * @param dst_edge <OUT> 出力したいエッジ画像格納先
	 * @param src_tracking <IN> 輪郭追跡結果画像
	 **/
	static void trackingResultToEdge(cv::Mat & dst_edge, const cv::Mat & src_tracking);
	
	/**
	 * @brief 与えられた方向(d)を逆向きにする
	 * @param d <IN> 逆にしたい方向
	 * @return 逆向き方向
	 **/
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

	/**
	 * @brief 注目画素(r,c)から方向先(to)の画素値を取得する
	 * @param bi_img <IN> 探索する二値画像(CV_8UC1)
	 * @param r <IN> bi_imgのrow座標。0~
	 * @param c <IN> bi_imgのcol座標。0~
	 * @param to <IN> 方向先。（例）注目画素の左の画素値が取得したいなら、LEFT
	 * @return 方向先の画素値
	 **/
	unsigned char getPixByDirection(const cv::Mat & bi_img, int r, int c, DIRECTION to);

	/**
	 * @brief 画素周囲にある 0でない画素値を持つ画素を見つける。1つ見つけたら終了。進入方向から時計回りに探索
	 * @param dst_to <OUT> 結果格納先
	 * @param bi_img <IN> 探索する二値画像(CV_8UC1)
	 * @param r <IN> bi_imgのrow座標。0~
	 * @param c <IN> bi_imgのcol座標。0~
	 * @param from <IN> 進入方向。（例）左からなら LEFT
	 * @param is_8_neighborhood <IN> (true) 8近傍探索 / 4近傍探索 (false)
	 * @return (true) 存在した(dst_toへ格納) / 存在しなかった (false)
	 **/
	bool searchValidPixAround(DIRECTION * dst_to, const cv::Mat & bi_img, int r, int c, DIRECTION from, bool is_8_neighborhood);

	/**
	 * @brief 再帰的に1画素ずつ追跡を行う処理部分（コア）。called by run()。
	 *        外部から直接呼び出すことはない予定。
	 *        TODO protected に移動予定
	 * @param bi_img <IN> 探索する二値画像(CV_8UC1)
	 * @param process <OUT> 探索の途中結果を格納する画像。事前にCV_8UC1で初期化されていること。
	 * @param r <IN> bi_imgのrow座標。0~
	 * @param c <IN> bi_imgのcol座標。0~
	 * @param from <IN> 進入方向。（例）左からなら LEFT
	 * @param is_8_neighborhood <IN> (true) 8近傍探索 / 4近傍探索 (false)
	 **/
	void recursiveContourTracking(const cv::Mat & bi_img, cv::Mat & process, int r, int c, DIRECTION from, bool is_8_neighborhood);

	/**
	 * @brief 輪郭追跡。時計回り。この関数を呼び出すのが基本
	 * @param dst <OUT> 追跡結果画像
	 * @param bi_img <IN> 二値画像(CV_8UC1)
	 * @param start_row <IN> 探索開始row座標 (0~
	 * @param start_col <IN> 探索開始col座標 (0~
	 * @param is_8_neighborhood 8近傍探索かどうか
	 * @return (true) 成功 / 失敗 (false)
	 **/
	bool run(cv::Mat & dst, const cv::Mat & bi_img, int start_row, int start_col, bool is_8_neighborhood = true);

};

/**
 * @brief 座標(r, c) が有効範囲内か
 * @param r <IN> row座標。0~
 * @param c <IN> col座標。0~
 * @param img_rows <IN> 対象画像のrow幅
 * @param img_cols <IN> 対象画像のcol幅
 * @return (true) 有効 / 範囲外 (false)
 **/
bool validCoord(int r, int c, int img_rows, int img_cols);
