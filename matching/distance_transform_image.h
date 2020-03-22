#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

void makeDistanceTransformImage(cv::Mat & dst, const cv::Mat & src, int start_row = 0, int start_col = 0);
