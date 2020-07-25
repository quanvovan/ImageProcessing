#pragma once
#include "opencv2\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include <iostream>
using namespace std;
using namespace cv;
class Transformer
{
public:
	/*
		srcImg : ảnh đầu vào
		desImg : ảnh kết quả 
		return 1 : ảnh chuyển đổi thành công
		return 0 : ảnh input bị lỗi hay không chuyển đổi thành công
	*/
	int Gray2RGB(Mat& srcImg, Mat& desImg);

	int RGB2Gray(Mat& srcImg, Mat& desImg);

	// tham số b : tăng độ sáng theo b 
	int ChangeBrighness(Mat& srcImg, Mat& desImg, int b);

	// tham số c : thay đổi độ tương phản theo c
	int ChangeContrast(Mat& srcImg, Mat& desImg, int c);
};

