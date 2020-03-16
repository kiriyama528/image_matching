
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


void contourTracking::run(cv::Mat &dst, const cv::Mat &bi_img, int start_row, int start_col, bool is_8_neighborhood = true) {
	UTYPE rows, cols, channels;
	if (getImgInfo(bi_img, &rows, &cols, &channels) == false) {
		fprintf(stderr, " > ERROR: ‰æ‘œ‚ª“Ç‚İ‚Ü‚ê‚Ä‚¢‚Ü‚¹‚ñ\n");
		return;
	}

	// Å‰‚Ì‰æ‘f‚ğŒ©‚Â‚¯‚Ä(r,c)‚ÉŠi”[‚·‚é
	int r = 0, c = 0;
	if (rasterScanForFirstValid(r, c, bi_img, 0, 0) == false) {
		fprintf(stderr, " > ERROR: ‰æ‘f’l‚ª0o‚È‚¢‰æ‘f‚ª‘¶İ‚µ‚Ü‚¹‚ñ‚Å‚µ‚½B\n");
		return;
	}


	// making
	// N“ü•ûŒü‚ğ¦‚µ‚Â‚ÂAüˆÍ’TõB
	// 4‹ß–T‚Æ8‹ß–T‚ÅüˆÍ‚Ì’è‹`‚ªˆá‚¤
	// MEMO ‚Æ‚è‚ ‚¦‚¸8‹ß–T‚ÌŒˆ‚ß‘Å‚¿‚Åì‚é
	/*
	recorsiveContourTracking(r, c, );

	// ‚Æ‚è‚ ‚¦‚¸void
	void recorsiveContourTracking(int r, int c, DIRECTION from, );
	serchValidPixAround(r, c, );
	*/
}