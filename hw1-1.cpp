#include <iostream>
#include <opencv2/opencv.hpp>
#include <sstream>
#include <cmath>
#include <stdlib.h>


using namespace cv;
using namespace std;

int main() {
	Mat img = imread("lena512.bmp.", CV_LOAD_IMAGE_UNCHANGED);
	Mat out_near;
	Mat out_bilinear;
	Mat out_rotate, rotate_x, rotate_y ;
	Mat lapalace1, lapalace2;
	

	//resize with nearest-neighbor interpolation(img_type:CV_8U)
	resize(img, out_near, Size(img.cols * 2, img.rows * 2), INTER_NEAREST);
	imwrite("out_near.bmp", out_near);

	//resize with a bilinear interpolation (used by default)(img_type:CV_8U)
	resize(img, out_bilinear, Size(img.cols * 2, img.rows * 2), INTER_LINEAR);
	imwrite("out_bilinear.bmp", out_bilinear);

	//img rotate(img_type:CV_8U)
	rotate_x.create(img.size(), CV_32FC1);
	rotate_y.create(img.size(), CV_32FC1);

	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			rotate_x.at<float>(i, j) = j;
			rotate_y.at<float>(i, j) = img.rows-i;
		}
	}
	remap(img, out_rotate, rotate_x, rotate_y, CV_INTER_LINEAR);
	imwrite("out_rotate.bmp", out_rotate);



	//bit-plane slicing(img_type:CV_8U)
	Mat money = imread("100-dollars.tif", CV_LOAD_IMAGE_UNCHANGED);
	Mat out(money.rows, money.cols, CV_8U, Scalar(0));
	
	for (int bitplane = 0; bitplane < 8; bitplane++) {
	    for (int i = 0; i < money.rows; i++) {
			for (int j = 0; j < money.cols; j++) {
				out.at<uchar>(i, j) = money.at<uchar>(i, j) % 2;
				money.at<uchar>(i, j) /= 2;
			}
		}
	out = out & 1;
	out = out * 255;

	string filename = "out_bit" + to_string(bitplane) + ".tif";
	imwrite(filename, out);
	}
	
	/*for (int i = 0; i < money.rows; i++) {
		for (int j = 0; j < money.cols; j++) {
			money.at<uchar>(i, j) /= 128; 
			//int test = money.at<uchar>(i, j);
			//out.at<uchar>(i, j) = (test>>7) % 2;
			out.at<uchar>(i, j) = money.at<uchar>(i, j) % 2;
			//out.at<uchar>(i, j) = out.at<uchar>(i, j) & 0xFF;
			if (100<i && i<= 111 && 100<j &&j <= 111) {
				printf("%d %d\n", money.at<uchar>(i, j)/256, out.at<uchar>(i, j));
			}
				
		}
	}
	
	out = out & 1;
	out = out * 255;
	
	string filename = "7.tif";
	imwrite(filename, out);*/

	//add bitplane 6,7,8
	Mat plane6 = imread("out_bit5.tif", CV_LOAD_IMAGE_GRAYSCALE);
	Mat plane7 = imread("out_bit6.tif", CV_LOAD_IMAGE_GRAYSCALE);
	Mat plane8 = imread("out_bit7.tif", CV_LOAD_IMAGE_GRAYSCALE);
	
	for (int i = 0; i < plane6.rows; i++) {
		for (int j = 0; j < plane6.cols; j++) {
			plane6.at<uchar>(i, j) /= 255;
			plane6.at<uchar>(i, j) *= 32;
		}
	}
	for (int i = 0; i < plane7.rows; i++) {
		for (int j = 0; j < plane7.cols; j++) {
			plane7.at<uchar>(i, j) /= 255;
			plane7.at<uchar>(i, j) *=64;
		}
	}
	for (int i = 0; i < plane8.rows; i++) {
		for (int j = 0; j < plane8.cols; j++) {
			plane8.at<uchar>(i, j) /= 255;
			plane8.at<uchar>(i, j) *= 128;
		}
	}
	bitwise_or(plane6, plane7, plane7);
	bitwise_or(plane7, plane8, plane8);
	imwrite("merge_6_7_8.tif",plane8 );

	

	//Lapalacian for blurry moon(img_type:CV_8U)
	Mat moon = imread("blurry_moon.tif", CV_LOAD_IMAGE_UNCHANGED);
	//GaussianBlur(moon, moon, Size(3, 3), 0, 0);
	Mat kernel = (Mat_<float>(3, 3) <<
		1, 1, 1,
		1, -8, 1,
		1, 1, 1);
	filter2D(moon, lapalace1, CV_32F, kernel);
	moon.convertTo(moon, CV_32F);
	lapalace2 = moon - lapalace1;
	lapalace1.convertTo(lapalace1, CV_8U);
	lapalace2.convertTo(lapalace2, CV_8U);
	imwrite("out_sharpmoon.tif", lapalace2);

	
	namedWindow("D1", WINDOW_AUTOSIZE);
	imshow("D1", out_near);
	namedWindow("D2", WINDOW_AUTOSIZE);
	imshow("D2", out_bilinear);
	namedWindow("D3", WINDOW_AUTOSIZE);
	imshow("D3", out_rotate);
	namedWindow("D4", WINDOW_AUTOSIZE);
	imshow("D4", plane8);
	namedWindow("D5", WINDOW_AUTOSIZE);
	imshow("D5", lapalace2);
	
	waitKey(0);
	//system("pause");
	return 0;
}