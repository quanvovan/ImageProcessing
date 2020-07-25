#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/nonfree/nonfree.hpp> 
#include <opencv2/legacy/legacy.hpp>
#include <vector>
#include <string>	
#include <cmath>
#include <stdio.h>

using namespace std;
using namespace cv;


class HarrisDetector
{
public:
	/*
	Phát hiện điện đặc trung sử dụng thuật toán hariss
	input: 
		- srcImg : ảnh truyền vào
		- k : tham số 
	output: 
	*/
	//Mat matrix_multiply(const Mat& Ix, const Mat& Iy); // độ tương quan các đạo hàm (tính các giá trị trong ma trận M)
	
	Mat detectHarrist(Mat srcImg, int blockSize, float k, float Threshold, vector<KeyPoint>& keypoint);


	HarrisDetector();
	~HarrisDetector();
};

