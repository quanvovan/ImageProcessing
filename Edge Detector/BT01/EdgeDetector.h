#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;
class EdgeDetector
{
private:
	int _lowThreshold ; // ngưỡng dưới
	int _highThreshold; // ngưỡng trên
public:
	/*
	Các hàm phát hiện biên cạnh
	sourceImage : ảnh input
	destinationImage : ảnh output

	kWidth, kHeight : kích thước kernel

	Giá trị trả về : 
		0 : nếu detect thành công
		1 : nếu detect thất bại 
	*/
	int DetectBySobel(const Mat& sourceImage, Mat& destinationImage, int kWidth, int kHeight);

	int DetectByPrewitt(const Mat& sourceImage, Mat& destinationImage, int kWidth, int kHeight);

	int DetectByLaplace(const Mat& sourceImage, Mat& destinationImage, int kWidth, int kHeight);

	int DetectByCanny(const Mat& sourceImage, Mat& destinationImage);
	void setThreshold(int low, int high); // low, hight : ngưỡng da người dùng nhập vào

};

