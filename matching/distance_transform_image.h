#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

void makeDistanceTransformImage(cv::Mat & dst, const cv::Mat & src);
