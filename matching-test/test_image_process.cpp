#include "pch.h"

#include "image_process.h"
#include "define.h"

// �_�~�[�֐�
int XXX() { return 0; }



// �摜���J���邩
//  �摜�̃p�X����������
TEST(UnitTestInputImage, validPath){
	const char *imgname = "C:\\Users\\kiriyama\\Pictures\\digital_image_processing\\template.jpg";
	bool expected = true;
	cv::Mat img;
	bool actual = readImage(imgname, img);
	EXPECT_EQ(expected, actual);
}


// �摜�̏���K�؂ɓǂݍ��߂Ă��邩
//  rows, cols, channels
TEST(UnitTestInputImage, correctInfo) {
	// �摜�̓ǂݍ���
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

// �_���ʂ�̉�f�i�`�����l���j�ɃA�N�Z�X�ł��邩
TEST(UnitTestInputImage, accessPixel) {
	// �摜�̓ǂݍ���
	const char *imgname = "C:\\Users\\kiriyama\\Pictures\\digital_image_processing\\template.jpg";
	cv::Mat img;
	readImage(imgname, img);
	
	// �摜�̍ŏ��̉�f�l���L�^
	unsigned char pix = at(img, 0, 0, RED);
	unsigned char before = pix;

	// �قȂ�l�ɏ��������������ŁA�ēx�擾����
	unsigned char expected = before + 1;
	at(img, 0, 0, RED) = expected;
	unsigned char actual= at(img, 0, 0, RED);
	
	// �����������Ă��邩
	EXPECT_NE(before, actual);

	// �\�z�ʂ�̒l���i�[����Ă��邩
	EXPECT_EQ(expected, actual);
}
