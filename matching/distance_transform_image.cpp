#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "image_process.h"
#include "distance_transform_image.h"
#include "contour_tracking.h"


void makeDistanceTransformImage(cv::Mat &dst, const cv::Mat &src, int start_row, int start_col) {
	UTYPE rows, cols, channels;
	getImgInfo(src, &rows, &cols, &channels);
	dst = cv::Mat::zeros(rows, cols, CV_8UC1);
	cv::Mat w_src = src.clone();  // ��Ɨp���͉摜

	unsigned char label_cnt = 1;
	cv::Mat track_result;
	contourTracking ct;
	while (ct.run(track_result, w_src, start_row, start_col, true)) {
		dst += track_result * label_cnt;
		
		// ���łɗ֊s�ǐՂ̌��ʂƂȂ����̈�͍폜
		for (int i = 0; i < rows*cols; i++) {
			if (track_result.data[i] == 1) {
				w_src.data[i] = 0;
			}
		}
		
		// ����ԍ����킩��悤�ɁA���x���ԍ����C���N�������g
		label_cnt++;
	}

}