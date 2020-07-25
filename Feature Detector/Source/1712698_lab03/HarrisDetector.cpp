#include "HarrisDetector.h"


Mat matrix_multiply(const Mat& Ix, const Mat& Iy)
{
	Mat res(Ix.rows, Ix.cols, CV_32F);
	//width là chiều rộng ảnh, height là chiều cao ảnh
	int height = res.rows, width = res.cols;
	float temp;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			temp = Ix.at<float>(i, j) * Iy.at<float>(i, j) * 1.0f;
			res.at<float>(i, j) = temp;
		}
	}
	return res;
}

// kiem tra x,y hop le
bool isInRange(int x, int y, int height, int width)
{
	if (x < 0 || y < 0 || x >= height || y >= width)
		return false;
	return true;
}

float calculateSumMat(const Mat& input, int blockSize, int x, int y)
{
	float res = 0.0f;
	int height = input.rows, width = input.cols;

	int b = blockSize / 2;
	for (int i = -b; i <= b; i++)
	{
		for (int j = -b; j <= b; j++)
		{
			if (isInRange(x+i,y+j,height,width))
				res += input.at<float>(x + i, y + j);
		}
	}
	//cout << res << endl;
	return res;
}


float findMax(Mat R, int i, int j,int blocksize)
{
	int b = blocksize / 2;
	int height = R.rows, width = R.cols;

	float maxPoint = 0.0f;
	for (int k = -b; k <= b; k++)
	{
		for (int l = -b; l <= b; l++)
		{
			if (isInRange(i + k, j + l, height, width))
			{
				if (maxPoint < R.at<float>(i + k, j + l))
					maxPoint = R.at<float>(i + k, j + l);

			}
		}
	}
	return maxPoint;
}

void nonmaxSuppression(Mat& R,int i,int j,float maxPoint,int blocksize, float Threshold)
{
	int b = blocksize / 2;
	int height = R.rows, width = R.cols;
	for (int k = -b; k <= b; k++)
	{
		for (int l = -b; l <= b; l++)
		{
			if (isInRange(i + k, j + l, height, width))
			{
				if ((R.at<float>(i + k, j + l) > Threshold))
				{
					//if ((R.at<float>(i + k, j + l) != maxPoint) || (R.at<float>(i + k, j + l) <= Threshold))
					if ((R.at<float>(i + k, j + l) == maxPoint))
						continue;
					else {
						R.at<float>(i + k, j + l) = 0;
					}
				}
				else R.at<float>(i + k, j + l) = 0;
			}
			
		}

	}
}

Mat ImageWithFeature(const Mat srcImg, const Mat& R, float Threshold,vector<KeyPoint> &keypoint)
{

	Mat dstImg;
	dstImg = srcImg.clone();

	uchar* pData = (uchar*)dstImg.data, * pRow;

	// width là chiều rộng mat R, height là chiều cao mat R
	int width = dstImg.cols, height = dstImg.rows;
	// widthStep là khoảng cách tính theo byte giữa 2 pixel cùng cột trên 2 dòng kế tiếp
	int widthStepDst = dstImg.step[0];

	float valR;

	for (int i = 0; i < height; i++, pData += widthStepDst)
	{
		pRow = pData;
		for (int j = 0; j < width; j++, pRow += 3)
		{
			valR = R.at<float>(i, j);
			if (valR > Threshold)
			{
				circle(dstImg, Point(j, i), 5.0, Scalar(0, 0, 255), 2, 8);
				keypoint.push_back(KeyPoint(Point(j, i),5.0));
			}
		}
	}
	return dstImg;
}

Mat HarrisDetector::detectHarrist(Mat srcImg, int blockSize, float k, float Threshold, vector<KeyPoint>& keypoint)
{
	if (srcImg.empty())
		return srcImg;
	Mat grayImg;
	cvtColor(srcImg, grayImg, COLOR_BGR2GRAY);
	//lọc nhiễu bằng Gauss
	GaussianBlur(grayImg, grayImg, Size(5, 5), 0, 0, BORDER_DEFAULT);
	
	/// width là chiều rộng ảnh, height là chiều cao ảnh
	int width = grayImg.cols, height = grayImg.rows;
	/// nChannels là số kênh màu
	int nChannels = grayImg.channels();
	/// widthStep là khoảng cách tính theo byte giữa 2 pixel cùng cột trên 2 dòng kế tiếp
	int widthStep = grayImg.step[0];

	/*=====================*/

	Mat Ix, Iy;
	Mat Ix2, Iy2, Ixy, Iyx;
	Mat Sx2, Sy2, Sxy, Syx;
	

	//1. tinh dao ham theo chieu x,y moi diem bang sobel (dung ham cua opencv da duoc cai dat o Lab2)
	/*	- src_gray: In our example, the input image. Here it is CV_8U
		- grad_x/grad_y: The output image.
		- ddepth: The depth of the output image. We set it to CV_16S to avoid overflow.
		- x_order: The order of the derivative in x direction.
		- y_order: The order of the derivative in y direction.
		- scale, delta and BORDER_DEFAULT: We use default values.
	*/

	
	Sobel(grayImg, Ix, CV_32F, 1, 0, 3);
	Sobel(grayImg, Iy, CV_32F, 0, 1, 3);
	
	
	// 2. do tuong quan giua cac dao ham
	// nhan cac diem tuong ung cua ma tran nay voi ma tran kia
	Ix2 = matrix_multiply(Ix, Ix);
	Iy2 = matrix_multiply(Iy, Iy);
	Ixy = matrix_multiply(Ix, Iy);
	Iyx = matrix_multiply(Iy, Ix);

	//imshow("Ix2", Ix2);
	//imshow("iy2", Iy2);
	//imshow("ixy", Ixy);
	////cout << srcImg.at<float>(0, 0);
	//waitKey(0);


	// ap dung mat na Gauss len cac ma tran thu duoc
	//Mat GIx2, GIy2, GIxy, GIyx;
	GaussianBlur(Ix2, Ix2, Size(5, 5), 1, 1, BORDER_DEFAULT);
	GaussianBlur(Iy2, Iy2, Size(5, 5), 1, 1, BORDER_DEFAULT);
	GaussianBlur(Ixy, Ixy, Size(5, 5), 1, 1, BORDER_DEFAULT);
	GaussianBlur(Iyx, Iyx, Size(5, 5), 1, 1, BORDER_DEFAULT);


	/*imshow("Ix2", Ix2);
	imshow("iy2", Iy2);
	imshow("ixy", Ixy);
	waitKey(0);*/

	//3. tinh corner response
	
	float SIx2, SIy2, SIxy, SIyx, lamda1, lamda2;
	Mat R(srcImg.rows, srcImg.cols,CV_32F);

	

	for (int i = 0; i < height; i++)
	{
		
		for (int j = 0; j < width; j++ )
		{
			// tinh tong ma tran kich thuoc trong blocksize tai vi tri i,j

			SIx2 = calculateSumMat(Ix2, blockSize, i, j);
			SIy2 = calculateSumMat(Iy2, blockSize, i, j);
			SIxy = calculateSumMat(Ixy, blockSize, i, j);
			SIyx = calculateSumMat(Iyx, blockSize, i, j);

			// ma tran M 
			//Mat M = (Mat_<float>(2, 2) << Sx2, Sxy, Syx, Sy2);
			///  R
			float traceM = SIx2 + SIy2;
			float detM = (SIx2 * SIy2) - (SIxy * SIyx);

			//cout << traceM << endl;
			//cout << detM << endl;
			R.at<float>(i, j) = detM - 0.05 * traceM * traceM;
			//cout << detM - 0.05 * traceM * traceM << endl;
		}
	}
	
	/*imshow("before", R);
	waitKey(0);*/
	// Hàm này sẽ giữ lại các vị trí có giá trị = giá trị max tại mỗi cửa sổ, các giá trị không thoả mãn sẽ bị đưa về 0
	int b = blockSize / 2;
	for (int i = 0; i < height ;i++)
	{
		for (int j = 0; j < width; j++)
		{
			float maxPoint = findMax(R, i, j, blockSize);
			nonmaxSuppression(R, i, j, maxPoint, blockSize, Threshold);
			
		}
	}

	R = ImageWithFeature(srcImg, R, 10000000000.0,keypoint);
	
	/*imshow("after", R);
	waitKey(0);*/
	return R;
}

HarrisDetector::HarrisDetector()
{
}

HarrisDetector::~HarrisDetector()
{
}
