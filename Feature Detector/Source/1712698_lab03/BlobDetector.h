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

using namespace cv;
using namespace std;

class BlobDetector
{
public: 
	

	/*
	Blob detection
	input:
	- srcImg : truyền vào ảnh
	- ksize : kích thước kernel trong bộ lọc laplace
	output : ảnh với các điểm đặc trưng blob
	*/
	Mat detectBlob(Mat srcImg, int ksize, vector<KeyPoint> &keypoint);

	BlobDetector();
	~BlobDetector();

};

