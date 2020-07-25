#pragma once
#include "BlobDetector.h"
#include "HarrisDetector.h"
#include "DoGDetector.h"
#include <opencv2/nonfree/nonfree.hpp> 
#include <opencv2/legacy/legacy.hpp>
# include <stdio.h>

class SiftDetector
{
public:
	vector<KeyPoint> keyPoint1;
	vector<KeyPoint> keyPoint2;

	int matchBySift(Mat img1, Mat img2, int detector,char *fileout);
	SiftDetector();
	~SiftDetector();
};

