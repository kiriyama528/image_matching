#pragma once

#define RED   0
#define GREEN 1
#define BLUE  2
#define ALPHA 3

typedef int UTYPE;

#define at(mat, r, c, bgra) (mat.data[(r*mat.cols + c)*mat.channels() + bgra])