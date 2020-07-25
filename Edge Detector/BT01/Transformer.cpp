#include "Transformer.h"

int Transformer::Gray2RGB(Mat& srcImg, Mat& desImg)
{	
	if (srcImg.empty())
		return 0;
	if (srcImg.type() == CV_8UC3)
	{
		desImg = srcImg;
		return 1;
	}

	int rows = srcImg.rows;
	int cols = srcImg.cols;

	desImg.create(srcImg.size(), CV_8UC3);

	for (int i = 0; i < rows; i++)
	{
		const uchar* src = srcImg.ptr<uchar>(i);
		uchar* res = desImg.ptr<uchar>(i);

		for (int j = 0; j < cols; j++, res += 3)
		{
			res[0] = (uchar)(src[j]);
			res[1] = (uchar)(src[j]);
			res[2] = (uchar)(src[j]);
		}
	}

	return 1;
}

int Transformer::RGB2Gray(Mat& srcImg, Mat& desImg)
{
	if (srcImg.empty())
		return 0;
	if (srcImg.type() == CV_8UC1)
	{
		desImg = srcImg;
		return 1;
	}

	desImg.create(srcImg.size(), CV_8UC1);

	int rows = srcImg.rows;
	int cols = srcImg.cols;

	for (int i = 0; i < rows; i++)
	{
		const uchar* src = srcImg.ptr<uchar>(i);
		uchar* res = desImg.ptr<uchar>(i);

		for (int j = 0; j < cols; j ++, src += 3)
		{
			res[j] = (uchar)(src[0] * 0.114f + src[1] * 0.587f + src[2] * 0.299f);
		}
	}
	return 1;
}

int Transformer::ChangeBrighness(Mat& srcImg, Mat& desImg, int b)
{
	if (srcImg.empty())
		return 0;
	
	int rows = srcImg.rows;
	int cols = srcImg.cols;

	desImg = srcImg.clone();
	//int nchannel = srcImg.channels;
	for (int i = 0; i < rows; i++)
	{
		const uchar* src = srcImg.ptr<uchar>(i);
		uchar* res = desImg.ptr<uchar>(i);

		for (int j = 0; j < cols; j++, src += 3, res += 3)
		{
			float blue = src[0] + b;
			float green = src[1] + b;
			float red = src[2] + b;

			blue = blue < 255 ? blue : 255;
			green = green < 255 ? green : 255;
			red = red < 255 ? red : 255;

			res[0] = (uchar)blue;
			res[1] = (uchar)green;
			res[2] = (uchar)red;
		}
	}
	return 1;
}

int Transformer::ChangeContrast(Mat& srcImg, Mat& desImg, int c)
{
	if (srcImg.empty())
		return 0;

	int rows = srcImg.rows;
	int cols = srcImg.cols;

	desImg = srcImg.clone();
	
	for (int i = 0; i < rows; i++)
	{
		const uchar* src = srcImg.ptr<uchar>(i);
		uchar* res = desImg.ptr<uchar>(i);

		for (int j = 0; j < cols; j++, src += 3, res += 3)
		{
			float blue = src[0] * c;
			float green = src[1] * c;
			float red = src[2] * c;

			blue = blue < 255 ? blue : 255;
			green = green < 255 ? green : 255;
			red = red < 255 ? red : 255;

			res[0] = (uchar)blue;
			res[1] = (uchar)green;
			res[2] = (uchar)red;
		}
	}
	return 1;

}
