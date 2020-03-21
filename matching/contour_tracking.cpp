
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "contour_tracking.h"
#include "define.h"
#include "image_process.h"

contourTracking::contourTracking()
{
}

contourTracking::~contourTracking()
{
}


void contourTracking::trackingResultToEdge(cv::Mat & dst_edge, const cv::Mat & src_tracking) {
	int rows = src_tracking.rows;
	int cols = src_tracking.cols;
	dst_edge = cv::Mat::zeros(rows, cols, CV_8UC1);
	unsigned char *dst_edge_data = dst_edge.data;
	unsigned char *src_tracking_data = src_tracking.data;
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < rows; c++) {
			int idx = r * cols + c; // channel数は1channelに決め打ち
			if (src_tracking_data[idx] != DEFAULT) {
				dst_edge_data[idx] = 1;
			}
		}
	}

}


contourTracking::DIRECTION contourTracking::reverseDirection(DIRECTION d) {
	// MEMO 不可。1になる → sizeof(DIRECTION) / sizeof(DEFAULT)
	const int n_DIRECTION = 9;  // TODO .hへ移動させるかも
	const DIRECTION src_dst[n_DIRECTION][2] = {
		{DEFAULT,    DEFAULT},
		{UP_LEFT,    DOWN_RIGHT},
		{UP,         DOWN},
		{UP_RIGHT,   DOWN_LEFT},
		{LEFT,       RIGHT},
		{RIGHT,      LEFT},
		{DOWN_LEFT,  UP_RIGHT},
		{DOWN,       UP},
		{DOWN_RIGHT, UP_LEFT}
	};

	for (int i = 0; i < n_DIRECTION; i++) {
		if (src_dst[i][0] == d) {
			return src_dst[i][1];
		}
	}
	
	// ERROR。ここにはたどりつかないはず
	// たどり着くとすれば、DIRECTIONを拡張したのに、この関数をメンテしてないとき
	// TODO ↓return を assertionに変更する
	return DEFAULT;
}


bool contourTracking::rasterScanForFirstValid(int & dst_r, int & dst_c, const cv::Mat & bi_img, int start_row, int start_col)
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


// TODO この関数の所属をはっきりさせる
bool validCoord(int r, int c, int img_rows, int img_cols) {
	if (r < 0 || img_rows <= r) {
		return false;
	}
	if (c < 0 || img_cols <= c) {
		return false;
	}

	return true;
}


bool contourTracking::directionToRC(const cv::Mat &img, int *dst_r, int *dst_c, int r, int c, DIRECTION to) {
	int move_r = 0, move_c = 0;
	switch (to) {
	case UP_LEFT:
		move_r = -1;
		move_c = -1;
		break;
	case UP:
		move_r = -1;
		move_c = 0;
		break;
	case UP_RIGHT:
		move_r = -1;
		move_c = 1;
		break;
	case LEFT:
		move_r = 0;
		move_c = -1;
		break;
	case RIGHT:
		move_r = 0;
		move_c = 1;
		break;
	case DOWN_LEFT:
		move_r = 1;
		move_c = -1;
		break;
	case DOWN:
		move_r = 1;
		move_c = 0;
		break;
	case DOWN_RIGHT:
		move_r = 1;
		move_c = 1;
		break;
	default:
		// do nothing
		break;
	}

	if (validCoord(r + move_r, c + move_c, img.rows, img.cols) == false) {
		// ERROR
		return false;
	}

	*dst_r = r + move_r;
	*dst_c = c + move_c;
	return true;
}


// 方向先の画素値を取得する
// TODO エラー時のreturn 値を見直す
unsigned char contourTracking::getPixByDirection(const cv::Mat & bi_img, int r, int c, DIRECTION to) {
	int dst_r = 0, dst_c = 0;
	if (directionToRC(bi_img, &dst_r, &dst_c, r, c, to) == false) {
		// ERROR
		return 0; // fix me 0でいいのか？本当に0の画素値が入っているときと見分けがつかない
	}

	return at(bi_img, dst_r, dst_c, 0);
}


// 周囲の 0 出ない画素を見つける
// 一つ見つけたら終了
// 進入方向から時計回りに探索する
bool contourTracking::searchValidPixAround(DIRECTION * dst_to, const cv::Mat &bi_img, int r, int c, DIRECTION from, bool is_8_neighborhood) {
	DIRECTION around8_idx[8] = { UP_LEFT, UP, UP_RIGHT, RIGHT, DOWN_RIGHT, DOWN, DOWN_LEFT, LEFT};
	DIRECTION around4_idx[4] = { UP, RIGHT, DOWN, LEFT };
	
	DIRECTION *around_idx;
	int n_idx;
	if (is_8_neighborhood) {
		around_idx = around8_idx;
		n_idx = 8;
	}
	else {
		around_idx = around4_idx;
		n_idx = 4;
	}
	
	int start_idx = -1;
	for (int i = 0; i < n_idx; i++) {
		if (around_idx[i] == from) {
			start_idx = i;
			break;
		}
	}

	if (start_idx == -1) {
		// ERROR
		return false;
	}
	
	for (int i = 1; i < n_idx; i++) {
		// リング状にアクセス
		int idx = (start_idx + i) % n_idx;
		unsigned char value = getPixByDirection(bi_img, r, c, around_idx[idx]);
		if (value != 0) {
			*dst_to = around_idx[idx];
			return true;
		}
	}

	return false;
}


// とりあえずvoid、8近傍決め打ち
// TODO 4近傍にも分岐できるようにする
void contourTracking::recursiveContourTracking(const cv::Mat & bi_img, cv::Mat & process, int r, int c, DIRECTION from, bool is_8_neighborhood) {
	// 終了判定
	if ( (at(process, r, c, 0) & from) == from) {
		return;
	}
	
	at(process, r, c, 0) = unsigned char (from);

	DIRECTION to = DEFAULT;
	if (searchValidPixAround(&to, bi_img, r, c, from, is_8_neighborhood)) {
		// 移動
		int next_r, next_c;
		directionToRC(bi_img, &next_r, &next_c, r, c, to);
		recursiveContourTracking(bi_img, process, next_r, next_c, reverseDirection(to), is_8_neighborhood);
	}

	// 末端へ到達したので終了。折り返さない
	return;
}


bool contourTracking::run(cv::Mat &dst, const cv::Mat &bi_img, int start_row, int start_col, bool is_8_neighborhood) {
	UTYPE rows, cols, channels;
	if (getImgInfo(bi_img, &rows, &cols, &channels) == false) {
		fprintf(stderr, " > ERROR: 画像が読み込まれていません\n");
		return false;
	}

	// 最初のエッジ画素を見つけて(start_row, start_col)を更新する
	if (rasterScanForFirstValid(start_row, start_col, bi_img, 0, 0) == false) {
		fprintf(stderr, " > ERROR: 画素値が0でない画素が存在しませんでした。\n");
		return false;
	}


	// making
	// 侵入方向を示しつつ、周囲探索。
	// 4近傍と8近傍で周囲の定義が違う
	// TODO 4近傍verの実装、フラグによる処理分岐	
	cv::Mat process = cv::Mat::zeros(rows, cols, CV_8UC1);  // 追跡途中データを格納する行列
	recursiveContourTracking(bi_img, process, start_row, start_col, LEFT, is_8_neighborhood);
	trackingResultToEdge(dst, process);
	return true;
}