#include "BlobDetector.h"

#define PI 3.141592653589793238462643383279502884L

bool checkPoint(int x, int y, int height, int width)
{
	if (x < 0 || y < 0 || x >= height || y >= width)
		return false;
	return true;
}


float findMaxPoint(Mat R, int i, int j, int blocksize,int type)
{
	int b = blocksize / 2;
	int height = R.rows, width = R.cols;

	float maxPoint = 0.0f;
	for (int k = -b; k <= b; k++)
	{
		for (int l = -b; l <= b; l++)
		{
			if (checkPoint(i + k, j + l, height, width))
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
float findMinPoint(Mat R, int i, int j, int blocksize, int type)
{
	int b = blocksize / 2;
	int height = R.rows, width = R.cols;

	float minPoint = 0.0f;
	for (int k = -b; k <= b; k++)
	{
		for (int l = -b; l <= b; l++)
		{
			if (checkPoint(i + k, j + l, height, width))
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

Mat BlobDetector::detectBlob(Mat srcImg, int ksize, vector<KeyPoint> &keypoint)
{
	
	Mat grayImage;
	cvtColor(srcImg, grayImage, COLOR_RGB2GRAY);
	grayImage.convertTo(grayImage, CV_32F);
	int height = grayImage.rows;
	int width = grayImage.cols;
	
	// chuan hoa anh ve binary (1,0)
	for(int i =0; i< height; i++)
		for (int j = 0; j < width; j++)
		{
			grayImage.at<float>(i, j) /= 255.0f;
		}

	
	//tao các cửa sổ ảnh : 
	// chon sigma theo : 
	//float sigma[7] = { 6.04965,7.38906,9.02501,11.0232,13.4637,16.4447,20.0855};
	// chọn 10 giá trị sigma
	vector<float> sigma;
	float s0 = 1.8;
	float delta = 0.2;
	for (int i = 0; i < 7; i++)
	{
		float temp = exp(s0);
		sigma.push_back(temp);
		//cout << sigma[i];
		s0 += delta;
		/*float temp = s0 * sqrt(2);
		sigma.push_back(temp);
		s0 = temp;*/
	}

	vector<Mat> gaussianImg;
	vector<Mat> LoGimg;
	Mat tempGauss;
	GaussianBlur(grayImage, tempGauss, Size(0, 0), sigma[0]);
	
	
	gaussianImg.push_back(tempGauss.clone());
	// áp dụng bộ lọc gauss cho các cửa sổ
	for (int i = 1; i < sigma.size(); i++)
	{
		GaussianBlur(gaussianImg[i - 1], tempGauss, Size(0, 0), sigma[i]);
		gaussianImg.push_back(tempGauss.clone());

	}
	// laplacian là đạo hàm bậc 2 của gaussian nên nhân với sigma^2
	for (int i = 0; i < sigma.size(); i++)
	{
		gaussianImg[i] *= sigma[i] * sigma[i];
	}

	// áp dụng bộ lọc laplace cho các cửa sổ đã lọc Gauss ở trên
	for (int i = 0; i < sigma.size(); i++)
	{
		Mat laplace;
		Laplacian(gaussianImg[i], laplace, CV_32F, ksize, 1, 0, BORDER_DEFAULT);
		LoGimg.push_back(laplace.clone());
		/*imshow("laplace", laplace);
		waitKey(1);*/
	}
	
	vector<int> scale;
	vector<Point> maximaPoints;
	// tim cực trị địa phương của hàm LoG 
	// result  tập các điểm (Point(x,y),scale) 
	
	for (int i = 0; i < sigma.size(); i++)
	{
		Mat up, middle, down;

		middle = LoGimg[i];

		if (i != 0)
		{
			down = LoGimg[i - 1];
		}
		if (i != sigma.size() - 1)
		{
			up = LoGimg[i + 1];
		}

		//waitKey(0);
		// tim cuc tri trên cửa số 3x3x3 trên miền biểu diễn không gian
		// 8 điêm xung quanh đối với middle
		// 9 điểm đối với up và down
		for (int h = 1; h < height -1; h++)
		{
			for (int w = 1; w < width-1; w++)
			{
				bool isMax = true;
				bool isMin = true;

				float currVal = middle.at<float>(h, w);

				int temp;
				temp = findMaxPoint(middle, h, w, 3, 1);
				temp = findMaxPoint(up, h, w, 3, 0);
				temp = findMaxPoint(down, h, w, 3, 0);

				if (currVal <= findMaxPoint(middle, h, w,3,1))
					isMax = false;

				if (currVal >= findMinPoint(middle, h, w, 3,1))
					isMin = false;
				
				if (i != 0)
				{
					if (currVal <= findMaxPoint(down, h, w, 3, 0))
						isMax = false;

					if (currVal >= findMinPoint(down, h, w, 3, 0))
						isMin = false;
				}
				if (i != sigma.size()-1)
				{
					if (currVal <= findMaxPoint(up, h, w, 3, 0))
						isMax = false;

					if (currVal >= findMinPoint(up, h, w, 3, 0))
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
		//cout << maximaPoints[i]<<",";
		//circle(srcImg, maximaPoints[i], r, Scalar(0, 0, 255), 1);
		keypoint.push_back(KeyPoint(maximaPoints[i], r));
	}
	drawKeypoints(srcImg, keypoint, srcImg, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	return srcImg;

}

BlobDetector::BlobDetector()
{
}

BlobDetector::~BlobDetector()
{
}
