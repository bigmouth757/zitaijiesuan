#include"core.h"
Mat ColorMask(const Mat& src, Scalar low, Scalar high)
{
	Mat hsv_img;
	cvtColor(src, hsv_img, COLOR_BGR2HSV);
	Mat mask;
	inRange(hsv_img, low, high, mask);
	Mat kernelC = getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat kernelO = getStructuringElement(MORPH_RECT, Size(3, 3));
	morphologyEx(mask, mask, MORPH_OPEN, kernelO);
	morphologyEx(mask, mask, MORPH_CLOSE, kernelC);
	return mask;
}