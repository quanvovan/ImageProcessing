//#include <highgui/highgui.hpp>  
//#include <opencv2/nonfree/nonfree.hpp> 
//#include <opencv2/legacy/legacy.hpp>  
//
//using namespace cv;
//using namespace std;
//
//int main()
//{
//    Mat image01 = imread("17.jpg");
//    Mat image02 = imread("18.jpg");
//    Mat image1, image2;
//    GaussianBlur(image01, image1, Size(3, 3), 0.5);
//    GaussianBlur(image02, image2, Size(3, 3), 0.5);
//
//    / / Extract feature points
//        SiftFeatureDetector siftDetector(30); //30 is the threshold of the detection operator
//    vector<KeyPoint> keyPoint1, keyPoint2;
//    siftDetector.detect(image1, keyPoint1);
//    siftDetector.detect(image2, keyPoint2);
//
//    / / draw feature points
//        drawKeypoints(image1, keyPoint1, image1, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
//    drawKeypoints(image2, keyPoint2, image2, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
//    namedWindow("KeyPoints of image1", 0);
//    namedWindow("KeyPoints of image2", 0);
//
//    imshow("KeyPoints of image1", image1);
//    imshow("KeyPoints of image2", image2);
//
//    / / Feature point description, prepare for the feature point matching below
//        SiftDescriptorExtractor siftDescriptor;
//    Mat imageDesc1, imageDesc2;
//    siftDescriptor.compute(image1, keyPoint1, imageDesc1);
//    siftDescriptor.compute(image2, keyPoint2, imageDesc2);
//
//    / / Feature points match and display matching results
//        BruteForceMatcher<L2<float>> matcher;
//    vector<DMatch> matchePoints;
//    matcher.match(imageDesc1, imageDesc2, matchePoints, Mat());
//    Mat imageOutput;
//    drawMatches(image01, keyPoint1, image02, keyPoint2, matchePoints, imageOutput);
//    namedWindow("Mathch Points", 0);
//    imshow("Mathch Points", imageOutput);
//    waitKey();
//    return 0;
//}