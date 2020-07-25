#pragma once
#include "opencv2\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc.hpp"
#include "opencv2\features2d.hpp"
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;


Mat convolved(Mat srcImg, vector <float> kernel, int ksize)
{
	Mat Convolution;
	int height = srcImg.rows, width = srcImg.cols;
	int n = ksize * ksize;
	int halfKsize = ksize / 2;
	vector<int> dx;
	vector<int> dy;
	// Khởi tạo ảnh kết quả convolution có kích thước bằng ảnh gốc.
	Convolution.create(height, width, CV_32F);
	// Khởi tạo offset kích thước bằng kernel.
	for (int i = halfKsize; i >= -halfKsize; i--)
		for (int j = halfKsize; j >= -halfKsize; j--)
		{
			dx.push_back(i);
			dy.push_back(j);
		}
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			float sum = 0.0;
			for (int k = 0; k < n; k++)
				if ((i + dx[k] >= 0) && (j + dy[k] >= 0) && (i + dx[k] < height) && (j + dy[k] < width))
					sum += srcImg.at<uchar>(i + dx[k], j + dy[k]) * kernel[k];

			Convolution.at<float>(i, j) = sum;
		}
	return Convolution;
	
}
