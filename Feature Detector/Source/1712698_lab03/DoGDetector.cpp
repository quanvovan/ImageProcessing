#include "DoGDetector.h"

#define PI 3.141592653589793238462643383279502884L

bool checkP(int x, int y, int height, int width)
{
	if (x < 0 || y < 0 || x >= height || y >= width)
		return false;
	return true;
}


float findMaxPoint_DoG(Mat R, int i, int j, int blocksize, int type)
{
	int b = blocksize / 2;
	int height = R.rows, width = R.cols;

	float maxPoint = 0.0f;
	for (int k = -b; k <= b; k++)
	{
		for (int l = -b; l <= b; l++)
		{
			if (checkP(i + k, j + l, height, width))
			{
				if (type == 1)//8 diem
				{
					if (k != 0 || l != 0)
						if (maxPoint < R.at<float>(i + k, j + l))
							maxPoint = R.at<float>(i + k, j + l);
				}
				if (type == 0) // 9 diem
				{
					if (maxPoint < R.at<float>(i + k, j + l))
						maxPoint = R.at<float>(i + k, j + l);
				}
			}
		}
	}
	return maxPoint;
}
float findMinPoint_DoG(Mat R, int i, int j, int blocksize, int type)
{
	int b = blocksize / 2;
	int height = R.rows, width = R.cols;

	float minPoint = 0.0f;
	for (int k = -b; k <= b; k++)
	{
		for (int l = -b; l <= b; l++)
		{
			if (checkP(i + k, j + l, height, width))
			{
				if (type == 1)//8 diem
				{
					if (k != 0 || l != 0)
						if (minPoint > R.at<float>(i + k, j + l))
							minPoint = R.at<float>(i + k, j + l);
				}
				if (type == 0) // 9 diem
				{
					if (minPoint > R.at<float>(i + k, j + l))
						minPoint = R.at<float>(i + k, j + l);
				}
			}
		}
	}
	return minPoint;
}

Mat DoGDetector::detectDoG(Mat srcImg, int ksize, vector<KeyPoint>& keypoint)
{

	Mat grayImage;
	cvtColor(srcImg, grayImage, COLOR_RGB2GRAY);
	grayImage.convertTo(grayImage, CV_32F);
	int height = grayImage.rows;
	int width = grayImage.cols;
	
	// chuan hoa anh ve binary (1,0)
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			grayImage.at<float>(i, j) /= 255.0f;
		}


	//tao các cửa sổ ảnh : 
	// chon sigma theo : 
	//float sigma[7] = { 6.04965,7.38906,9.02501,11.0232,13.4637,16.4447,20.0855};
	vector<float> sigma;
	float s0 = 4;
	float delta = 0.2;
	for (int i = 0; i < 5; i++)
	{
		//float temp = exp(s0);
		//sigma.push_back(temp);
		////cout << sigma[i];
		//s0 += delta;

		float temp = s0 * sqrt(2);
		sigma.push_back(temp);
		s0 = temp;

	}

	vector<Mat> gaussianImg;
	vector<Mat> DoGimg;
	Mat tempGauss1, tempGauss2;
	Mat tempGauss;
	GaussianBlur(grayImage, tempGauss1, Size(0, 0), sigma[0]);
	GaussianBlur(tempGauss1, tempGauss2, Size(0, 0), sigma[1]);
	tempGauss = tempGauss1 - tempGauss2;
	DoGimg.push_back(tempGauss.clone());
	// áp dụng bộ lọc gauss cho các cửa sổ
	for (int i = 1; i < sigma.size(); i++)
	{
		GaussianBlur(DoGimg[i - 1], tempGauss1, Size(0, 0), sigma[i-1]);
		GaussianBlur(DoGimg[i - 1], tempGauss2, Size(0, 0), sigma[i]);

		tempGauss = tempGauss1 - tempGauss2;

		DoGimg.push_back(tempGauss.clone());
	
	}
	
	vector<int> scale;
	vector<Point> maximaPoints;
	// tim cực trị địa phương của hàm LoG 
	// result  tập các điểm (Point(x,y),scale) 

	for (int i = 0; i < sigma.size(); i++)
	{
		Mat up, middle, down;

		middle = DoGimg[i];

		if (i != 0)
		{
			down = DoGimg[i - 1];
			//imshow("down", down);
		}
		if (i != sigma.size() - 1)
		{
			up = DoGimg[i + 1];
			//imshow("up", up);
		}
		//imshow("middle", middle);

		//waitKey(0);
		// tim cuc tri trên cửa số 3x3x3 trên miền biểu diễn không gian
		// 8 điêm xung quanh đối với middle
		// 9 điểm đối với up và down
		for (int h = 1; h < height - 1; h++)
		{
			for (int w = 1; w < width - 1; w++)
			{
				bool isMax = true;
				bool isMin = true;

				float currVal = middle.at<float>(h, w);

				int temp;
				temp = findMaxPoint_DoG(middle, h, w, 3, 1);
				temp = findMaxPoint_DoG(up, h, w, 3, 0);
				temp = findMaxPoint_DoG(down, h, w, 3, 0);

				if (currVal <= findMaxPoint_DoG(middle, h, w, 5, 1))
					isMax = false;

				if (currVal >= findMinPoint_DoG(middle, h, w, 5, 1))
					isMin = false;

				if (i != 0)
				{
					if (currVal <= findMaxPoint_DoG(down, h, w, 5, 0))
						isMax = false;

					if (currVal >= findMinPoint_DoG(down, h, w, 5, 0))
						isMin = false;
				}
				if (i != sigma.size() - 1)
				{
					if (currVal <= findMaxPoint_DoG(up, h, w, 5, 0))
						isMax = false;

					if (currVal >= findMinPoint_DoG(up, h, w, 5, 0))
						isMin = false;
				}

				if (isMax || isMin)
				{
					//dstImg.at<float>(w, h) = 1.0f;
					maximaPoints.push_back(Point(w, h)); // các điểm cực trị trong cửa sổ
					scale.push_back(i); // đánh dấu các cửa sổ
				}
				/*else
					dstImg.at<float>(w,h) = 0.0f;*/
			}
		}
	}


	for (int i = 0; i < scale.size(); i++)
	{
		// sigma = r/can2
		float r = sqrt(2) * sigma[scale[i]];
		//circle(srcImg, maximaPoints[i], r, Scalar(0, 0, 255), 1, 8);
		keypoint.push_back(KeyPoint(maximaPoints[i],r));
		
	}
	drawKeypoints(srcImg, keypoint, srcImg, Scalar(0, 0, 255),DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	
	return srcImg;

}



DoGDetector::DoGDetector()
{
}

DoGDetector::~DoGDetector()
{
}
