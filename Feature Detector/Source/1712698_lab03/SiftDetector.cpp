#include "SiftDetector.h"

int SiftDetector::matchBySift(Mat img1, Mat img2, int detector,char* fileout)
{
	Mat desImg;
	Mat Feature1, Feature2;
	//Mat img1, img2;
	;
	// lấy điểm đặc trưng
	if (detector == 1)
	{
		HarrisDetector harris;
		Feature1 = harris.detectHarrist(img1, 5, 0.05, 10000.0, keyPoint1);
		Feature2 = harris.detectHarrist(img2, 5, 0.05, 10000.0, keyPoint2);
	}
	else if (detector == 2)
	{
		BlobDetector blob;
		Feature1 = blob.detectBlob(img1, 5, keyPoint1);
		Feature2 = blob.detectBlob(img2, 5, keyPoint2);
	}
	else
	{
		DoGDetector DoG;
		Feature1 = DoG.detectDoG(img1, 5, keyPoint1);
		Feature2 = DoG.detectDoG(img2, 5, keyPoint2);
	}
	
	SiftDescriptorExtractor siftfeature;
	Mat imgDesc1, imgDesc2;
	//Feature point description, prepare for the feature point matching below
	siftfeature.compute(img1, keyPoint1, imgDesc1);
	siftfeature.compute(img2, keyPoint2, imgDesc2);
	//Feature points match and display matching results
	BFMatcher bf(NORM_L2, true);

	BruteForceMatcher<L2<float>> matcher;
	vector<vector<DMatch>> matchePoints;
	//matcher.match(imgDesc1, imgDesc2, matchePoints, Mat());
	bf.knnMatch(imgDesc1, imgDesc2, matchePoints, 1);

	drawMatches(img1, keyPoint1, img2, keyPoint2, matchePoints, desImg);
	//drawMatches(img1, keyPoint1, img2, keyPoint2, match2, des2);
	namedWindow("Mathch Points1", 0); //namedWindow("Mathch Points2", 0);
	imshow("Mathch Points1", desImg);
	imwrite(fileout, desImg);
	waitKey(0);
	
	return 1;
}

SiftDetector::SiftDetector()
{
}

SiftDetector::~SiftDetector()
{
}
