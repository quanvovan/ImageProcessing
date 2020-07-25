#include "EdgeDetector.h"

int EdgeDetector::DetectBySobel(const Mat& sourceImage, Mat& destinationImage, int kWidth, int kHeight)
{
	if (sourceImage.empty())
		return 0;

	Mat grayImg;
	// chuyển sang ảnh xám 
	if (sourceImage.type() != CV_8UC1)
		cvtColor(sourceImage, grayImg, COLOR_BGR2GRAY);
	else
		sourceImage.convertTo(grayImg, CV_8UC1);

	//làm trơn ảnh bằng Gauss
	GaussianBlur(grayImg, grayImg, Size(5, 5), 0);
	
	/*-----------------------------------------------------*/

	destinationImage.create(grayImg.size(), grayImg.type());


	vector<vector<int>> matrix;
	matrix.resize(grayImg.rows);
	for (int i = 0; i < grayImg.rows; i++)
	{// Lưu ma trận của ảnh sourceImage
		matrix[i].resize(grayImg.cols);
		for (int k = 0; k < grayImg.cols; k++)
		{
			matrix[i][k] = static_cast<int>(grayImg.at<uchar>(i, k));
		}
	}

	for (int i = 1; i < grayImg.rows - 1; i++)
	{// Tính toán theo mặt nạ Sobel để có được biên cạnh
		for (int k = 1; k < grayImg.cols - 1; k++)
		{
			int Gx = matrix[i - 1][k + 1] + 2 * matrix[i][k + 1] + matrix[i + 1][k + 1];
			Gx -= matrix[i - 1][k - 1] + 2 * matrix[i][k - 1] + matrix[i + 1][k - 1];
			int Gy = matrix[i + 1][k - 1] + 2 * matrix[i + 1][k] + matrix[i + 1][k + 1];
			Gy -= matrix[i - 1][k - 1] + 2 * matrix[i - 1][k] + matrix[i - 1][k + 1];

			float alpha = round(sqrt(Gx * Gx + Gy * Gy));// Có thể chọn alpha = |Gx| + |Gy|
			destinationImage.at<uchar>(i, k) = static_cast<uchar>(alpha);
		}
	}
	return 1;
}

int EdgeDetector::DetectByPrewitt(const Mat& sourceImage, Mat& destinationImage, int kWidth, int kHeight)
{
	if (sourceImage.empty())
		return 0;

	Mat grayImg;
	// chuyển sang ảnh xám 
	if (sourceImage.type() != CV_8UC1)
		cvtColor(sourceImage, grayImg, COLOR_BGR2GRAY);
	else
		sourceImage.convertTo(grayImg, CV_8UC1);

	//làm trơn ảnh bằng Gauss
	GaussianBlur(grayImg, grayImg, Size(5, 5), 0);

	/*-----------------------------------------------------*/
	destinationImage.create(grayImg.size(), grayImg.type());
	vector<vector<int>> matrix;
	matrix.resize(grayImg.rows);
	for (int i = 0; i < grayImg.rows; i++)
	{// Lưu ma trận của ảnh sourceImage
		matrix[i].resize(grayImg.cols);
		for (int k = 0; k < grayImg.cols; k++)
		{
			matrix[i][k] = static_cast<int>(grayImg.at<uchar>(i, k));
		}
	}

	for (int i = 1; i < grayImg.rows - 1; i++)
	{// Tính toán theo mặt nạ Prewitt để có được biên cạnh
		for (int k = 1; k < grayImg.cols - 1; k++)
		{
			int Gx = matrix[i - 1][k + 1] + matrix[i][k + 1] + matrix[i + 1][k + 1];
			Gx -= matrix[i - 1][k - 1] + matrix[i][k - 1] + matrix[i + 1][k - 1];


			int Gy = matrix[i + 1][k - 1] + matrix[i + 1][k] + matrix[i + 1][k + 1];
			Gy -= matrix[i - 1][k - 1] + matrix[i - 1][k] + matrix[i - 1][k + 1];

			float alpha = round(sqrt(Gx * Gx + Gy * Gy));// Có thể chọn alpha = |Gx| + |Gy|
			destinationImage.at<uchar>(i, k) = static_cast<uchar>(alpha);
		}
	}

	return 1;
}

int EdgeDetector::DetectByLaplace(const Mat& sourceImage, Mat& destinationImage, int kWidth, int kHeight)
{
	if (sourceImage.empty())
		return 0;

	Mat grayImg;
	// chuyển sang ảnh xám 
	if (sourceImage.type() != CV_8UC1)
		cvtColor(sourceImage, grayImg, COLOR_BGR2GRAY);
	else
		sourceImage.convertTo(grayImg, CV_8UC1);

	//làm trơn ảnh bằng Gauss
	GaussianBlur(grayImg, grayImg, Size(5, 5), 0);

	/*-----------------------------------------------------*/

	destinationImage.create(grayImg.size(), grayImg.type());
	vector<vector<int>> matrix;
	matrix.resize(grayImg.rows);
	for (int i = 0; i < grayImg.rows; i++)
	{// Lưu ma trận của ảnh sourceImage
		matrix[i].resize(grayImg.cols);
		for (int k = 0; k < grayImg.cols; k++)
		{
			matrix[i][k] = static_cast<int>(grayImg.at<uchar>(i, k));
		}
	}
	for (int i = 1; i < grayImg.rows - 1; i++)
	{// Tính toán theo mặt nạ Laplace để có được biên cạnh
		for (int k = 1; k < grayImg.cols - 1; k++)
		{
			//int G = matrix[i - 1][k] + matrix[i + 1][k] + matrix[i][k - 1] + matrix[i][k + 1] - 8 * matrix[i][k];
			int G = matrix[i - 1][k + 1] + matrix[i][k + 1] + matrix[i + 1][k + 1];
			G += matrix[i - 1][k - 1] + matrix[i][k - 1] + matrix[i + 1][k - 1];
			G += matrix[i - 1][k] - 8 * matrix[i][k] + matrix[i + 1][k];

			destinationImage.at<uchar>(i, k) = saturate_cast<uchar>(G);
		}
	}

	return 1;
}


void Hysterisis(const Mat& G, Mat& destinationImage, int lowThreshold, int highThreshold, int x, int y)
{
	if (x < 0 || y < 0 || x >= destinationImage.rows || y >= destinationImage.cols || destinationImage.at<uchar>(x, y) != 0)
		return;
	// xét vị trí ảnh tại vị trí (x,y)
	// nếu G(x,Y)<lowThreshold => loại bỏ điểm này 
	if (G.at<float>(x, y) < highThreshold)
	{
		//destinationImage.at<uchar>(x, y) = 0;
		return;
	}
	else
	{
		destinationImage.at<uchar>(x, y) = 255;
		return;
	}


	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			int index = (x + 1) * 3 + y + 1;
			Hysterisis(G, destinationImage, lowThreshold, highThreshold, i + x, j + y);

		}
	}
}


int EdgeDetector::DetectByCanny(const Mat& sourceImage, Mat& destinationImage)
{
	if (sourceImage.empty())
		return 0;

	Mat grayImage;
	// chuyển sang ảnh xám 
	if (sourceImage.type() != CV_8UC1)
		cvtColor(sourceImage, grayImage, COLOR_BGR2GRAY);
	else
		sourceImage.convertTo(grayImage, CV_8UC1);
	//grayImage = sourceImage.clone();


	//chiều rộng và chiều cao
	int width = grayImage.cols, height = grayImage.rows;
	//nChannels là số kênh màu
	int nChannels = grayImage.channels();
	//widthStep là khoảng cách tính theo byte giữa 2 pixel cùng cột trên 2 dòng kế tiếp
	int widthStep = grayImage.step[0];

	//Khởi tạo ảnh đích có kích thước và type giống ảnh Gray
	destinationImage = Mat::zeros(grayImage.size(), CV_8UC1);

	// bước 1 : làm trơn ảnh bằng Gauss
	GaussianBlur(grayImage, grayImage, Size(5, 5), 0);

	// tao ma tran offset
	vector<int> offsets;
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			offsets.push_back(widthStep * i + j);
		}
	}
	uchar* pGray = (uchar*)grayImage.data;

	// bước 2 : Tính đạo hàm theo hướng của ảnh Gray ở trên
	//kernel  toán tử sobel 
	float kernel_x[9] = { -1.0f, 0.0f, 1.0f, -2.0f, 0.0f, 2.0f, -1.0f, 0.0f, 1.0f };
	float kernel_y[9] = { 1.0f, 2.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, -2.0f, -1.0f };

	Mat Gx(grayImage.size(), CV_32FC1); // đạo hàm theo hướng x
	Mat Gy(grayImage.size(), CV_32FC1); // đạo hàm theo hướng y
	Mat G(grayImage.size(), CV_32FC1); // Gradient
									   // ảnh Non maximum suppression
	Mat angleImage(grayImage.size(), grayImage.type());

	//Gx = cv::Sobel(grayImage, CV_64F, 1, 0, CV_MAX_SOBEL_KSIZE);
	//Gy = cv::Sobel(grayImage, CV_64F, 0, 1, CV_MAX_SOBEL_KSIZE);
	for (int i = 0; i < height; i++, pGray += widthStep)
	{
		uchar* pGrayRow = pGray;
		for (int j = 0; j < width; j++, pGrayRow += nChannels)
		{
			float sum_x = 0.0f;
			float sum_y = 0.0f;
			for (int x = -1; x <= 1; x++)
			{
				for (int y = -1; y <= 1; y++)
				{
					int index = (x + 1) * 3 + (y + 1); // Vi tri trong mang kernel
					sum_x += pGrayRow[offsets[index]] * kernel_x[index];
					sum_y += pGrayRow[offsets[index]] * kernel_y[index];
				}
			}
			// gán vào ảnh đạo hàm
			float sum = sqrtf(powf(sum_x, 2) + powf(sum_y, 2));

			Gx.at<float>(i, j) = sum_x;
			Gy.at<float>(i, j) = sum_y;
			G.at<float>(i, j) = sum;
		}
	}

	//bước 3: Loại bỏ một số điểm dư thừa

	//tìm hướng gradient
	// mỗi pixel trên ma trận angle thể hiện hướng của cạnh
	// chia các góc về 4 trường hợp đại diện cho 4 hướng : 
	// hướng ngang(0 độ), hướng chéo bên phải(45 độ), hướng dọc(90 độ), hướng chéo trái(135 độ)
	float X, Y, Theta, Gp, Ga, Gb;
	uchar* pAngle = (uchar*)angleImage.data, * pAngleRow;
	for (int i = 0; i < height; i++, pAngle += widthStep)
	{
		pAngleRow = pAngle;
		for (int j = 0; j < width; j++, pAngleRow += nChannels)
		{
			X = Gx.at<float>(i, j);
			Y = Gy.at<float>(i, j);
			//Tính hướng
			Theta = fastAtan2(Y, X);
			Gp = G.at<float>(i, j);

			if (Theta <= 22.5 || Theta > 157.5) // 0 độ
			{
				//(x, y-1) (x, y+1)
				if (j - 1 >= 0)
					Ga = G.at<float>(i, j - 1);
				if (j + 1 < width)
					Gb = G.at<float>(i, j + 1);
			}
			else if (Theta > 22.5 && Theta <= 67.5) // 45 độ
			{
				//(x - 1, y + 1), (x + 1, y - 1)
				if (j + 1 < width && i - 1 >= 0)
					Ga = G.at<float>(i - 1, j + 1);
				if (j - 1 >= 0 && i + 1 < height)
					Gb = G.at<float>(i + 1, j - 1);
			}
			else if (Theta > 67.5 && Theta <= 112.5) // 90 độ
			{
				//(x-1, y), (x+1, y)
				if (i - 1 >= 0)
					Ga = G.at<float>(i - 1, j);
				if (i + 1 < height)
					Gb = G.at<float>(i + 1, j);
			}
			else // 135 độ
			{
				//(x - 1, y - 1), (x + 1, y + 1)
				if (j - 1 >= 0 && i - 1 >= 0)
					Ga = G.at<float>(i - 1, j - 1);
				if (j + 1 < width && i + 1 < height)
					Gb = G.at<float>(i + 1, j + 1);
			}

			//Non-maximal suppression
			//đánh dấu các điểm loại nếu điểm đang xét nhỏ hơn một trong 2 điểm hướng
			if (Gp < Ga || Gp < Gb)
				pAngleRow[0] = 0;
			else
				pAngleRow[0] = 1;
		}
	}

	//nếu pangle = 0 thì loại bớt điểm đó đi 
	pAngle = (uchar*)angleImage.data;
	for (int i = 0; i < height; i++, pAngle += widthStep)
	{
		pAngleRow = pAngle;
		for (int j = 0; j < width; j++, pAngleRow += nChannels)
		{
			if (pAngleRow[0] == 0)
				G.at<float>(i, j) = 0;
		}
	}

	//bước 4: áp dụng ngưỡng
	int* edges = new int[height * width];
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{

			Gp = G.at<float>(i, j);
			if (destinationImage.at<uchar>(i, j) == 0)
			{
				if (Gp >= _highThreshold)
				{
					destinationImage.at<uchar>(i, j) = 255;
				}
				else if (Gp < _lowThreshold)
				{
					destinationImage.at<uchar>(i, j) = 0;
				}
				else
				{
					Hysterisis(G, destinationImage, _lowThreshold, _highThreshold, i, j);
				}
			}

		}
	}


	grayImage.release();
	G.release(); G.release();
	Gy.release();
	angleImage.release();

	if (destinationImage.empty())
		return 0;
	return 1;
}

void EdgeDetector::setThreshold(int low, int high)
{
	_lowThreshold = low * 0.8;
	_highThreshold = high * 0.6;
}
