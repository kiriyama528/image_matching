
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


contourTracking::DIRECTION contourTracking::reverseDirection(DIRECTION d) {
	const DIRECTION src_dst[][2] = {
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

	for (int i = 0; i < sizeof(DIRECTION); i++) {
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

bool validCoord(int r, int c, int img_rows, int img_cols) {
	if (r < 0 && img_rows <= r) {
		return false;
	}
	if (c < 0 && img_cols <= r) {
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

	r = r + move_r;
	c = c + move_c;
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
	const DIRECTION around8_idx[] = { UP_LEFT, UP, UP_RIGHT, RIGHT, DOWN_RIGHT, DOWN, DOWN_LEFT, LEFT};
	const DIRECTION around4_idx[] = { UP, RIGHT, DOWN, LEFT };
	
	const DIRECTION *around_idx;
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

	if (start_idx = -1) {
		// ERROR
		return false;
	}
	
	for (int i = 0; i < n_idx; i++) {
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
void contourTracking::recorsiveContourTracking(const cv::Mat & bi_img, cv::Mat & process, int r, int c, DIRECTION from, bool is_8_neighborhood) {
	// 終了判定
	if (at(process, r, c, 0) == from) {
		return;
	}
	
	at(process, r, c, 0) = unsigned char (from);

#if 1 // 本実装
	DIRECTION to = DEFAULT;
	if (searchValidPixAround(&to, bi_img, r, c, from, is_8_neighborhood)) {
		// 移動
		int next_r, next_c;
		directionToRC(bi_img, &next_r, &next_c, r, c, to);
		return recorsiveContourTracking(bi_img, process, next_r, next_c, reverseDirection(to), is_8_neighborhood);
	}

#else // 仮のGREENコード
	unsigned char img[5*5] = {
		0, 0, 0, 0, 0,
		0, 1, 1, 1, 0,
		0, 1, 0, 1, 0,
		0, 1, 1, 1, 0,
		0, 0, 0, 0, 0
	};

	memcpy(process.data, img, 5 * 5);
	
#endif
}


void contourTracking::run(cv::Mat &dst, const cv::Mat &bi_img, int start_row, int start_col, bool is_8_neighborhood) {
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


	// making
	// 侵入方向を示しつつ、周囲探索。
	// 4近傍と8近傍で周囲の定義が違う
	
	cv::Mat process = cv::Mat::zeros(rows, cols, CV_8UC1);  // 追跡途中データを格納する行列
	recorsiveContourTracking(bi_img, process, r, c, LEFT, is_8_neighborhood);
	
}