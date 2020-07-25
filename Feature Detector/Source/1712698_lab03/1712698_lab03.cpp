// 1712698_lab03.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "HarrisDetector.h"
#include "DoGDetector.h"
#include "BlobDetector.h"
#include "SiftDetector.h"

int main(int argc, char* argv[])
{
#pragma region ONLY_FOR_DEBUG
	//argc = 6;
	//char* temp[6];
	//temp[0] = _strdup("1712698_lab03.exe");
	//temp[1] = _strdup("02_1.jpg");
	//temp[2] = _strdup("02_3.jpg");
	//temp[3] = _strdup("--matchBySift");
	//temp[4] = _strdup("5");
	//temp[5] = _strdup("Match_011_012.jpg");
	//argv = temp;
#pragma endregion
	char* command, * inputPath,* outputPath,* cmdArguments;

	try
	{
		inputPath = argv[1];
		outputPath = argv[2];
		command = argv[3];

		Mat srcImg = imread(inputPath, IMREAD_ANYCOLOR);
		Mat desImg;
		Mat grayImg;
		vector<KeyPoint> featurePoint;
		//cvtColor(srcImg, grayImg, COLOR_BGR2GRAY);
		namedWindow("Display", WINDOW_AUTOSIZE);
		if (strcmp(command, "--harris") == 0)
		{
			HarrisDetector harris;
			int ksize = atoi(argv[4]);
			float threshold = atof(argv[5]);
			desImg = harris.detectHarrist(srcImg, ksize, 0.05, threshold, featurePoint);
			

		}
		if (strcmp(command, "--dog") == 0)
		{
			DoGDetector dog;
			int ksize = atoi(argv[4]);
			desImg = dog.detectDoG(srcImg, ksize, featurePoint);
		}

		if (strcmp(command, "--blob") == 0)
		{
			
			BlobDetector blob;
			int ksize = atoi(argv[4]);
			desImg = blob.detectBlob(srcImg, ksize, featurePoint);
			
		}

		// chua lam
		if (strcmp(command, "--matchBySift") == 0)
		{
			char* fileout = argv[5];
			SiftDetector match;

			Mat img2 = imread(outputPath, IMREAD_ANYCOLOR);
			int check = match.matchBySift(srcImg, img2,1,fileout);
			
		}

		if (!desImg.empty())
		{
			imshow("input", srcImg);
			imshow("Image with Feature", desImg);
			imwrite(outputPath, desImg);
			cout<< "completed........... ";
			waitKey(0);
		}
		else
		{
			throw "Error.........";
		}
		
	}
	catch (const char* msg)
	{
		cout << msg << endl;
		system("pause");
	}

	//
#pragma region ONLY_FOR_DEBUG
	/*free(temp[0]);
	free(temp[1]);
	free(temp[2]);
	free(temp[3]);
	free(temp[4]);
	free(temp[5]);*/
	
#pragma endregion

	

	//system("pause");
	return 0;

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file