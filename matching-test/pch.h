//
// pch.h
// Header for standard system include files.
//

#pragma once

#include "gtest/gtest.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

bool isEqualMat(const cv::Mat mat1, const cv::Mat mat2);
