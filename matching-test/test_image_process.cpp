#include "pch.h"

#include "image_process.h"
#include "define.h"

// ダミー関数
int XXX() { return 0; }



// 画像を開けるか
//  画像のパスが正しいか
TEST(UnitTestInputImage, validPath){
	const char *imgname = "C:\\Users\\kiriyama\\Pictures\\digital_image_processing\\template.jpg";
	bool expected = true;
	cv::Mat img;
	bool actual = readImage(imgname, img);
	EXPECT_EQ(expected, actual);
}


// 画像の情報を適切に読み込めているか
//  rows, cols, channels
TEST(UnitTestInputImage, correctInfo) {
	// 画像の読み込み
	const char *imgname = "C:\\Users\\kiriyama\\Pictures\\digital_image_processing\\template.jpg";
	cv::Mat img;
	readImage(imgname, img);

	int expected_rows = 2276;
	int expected_cols = 1591;
	int expected_channels = 3;

	int act_rows, act_cols, act_channels;
	getImgInfo(img, &act_rows, &act_cols, &act_channels);

	EXPECT_EQ(expected_rows, act_rows);
	EXPECT_EQ(expected_cols, act_cols);
	EXPECT_EQ(expected_channels, act_channels);
}

// 狙い通りの画素（チャンネル）にアクセスできるか
TEST(UnitTestInputImage, accessPixel) {
	// 画像の読み込み
	const char *imgname = "C:\\Users\\kiriyama\\Pictures\\digital_image_processing\\template.jpg";
	cv::Mat img;
	readImage(imgname, img);
	
	// 画像の最初の画素値を記録
	unsigned char pix = at(img, 0, 0, RED);
	unsigned char before = pix;

	// 異なる値に書き換えたうえで、再度取得する
	unsigned char expected = before + 1;
	at(img, 0, 0, RED) = expected;
	unsigned char actual= at(img, 0, 0, RED);
	
	// 書き換えられているか
	EXPECT_NE(before, actual);

	// 予想通りの値が格納されているか
	EXPECT_EQ(expected, actual);
}
