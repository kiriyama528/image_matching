#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "image_process.h"

using namespace cv;



int main()
{
	const char* imgname = "C:\\Users\\kiriyama\\Pictures\\digital_image_processing\\img1.JPG";
	const char* tmpname = "C:\\Users\\kiriyama\\Pictures\\digital_image_processing\\template.jpg";
	Mat img;
	Mat temp_img;
	readImage(imgname, img);
	readImage(tmpname, temp_img);

	
	Mat image = Mat::zeros(100, 100, CV_8UC3);
	imshow("", image);
	waitKey(0);
}