// OPENCV_03.cpp : Bu dosya 'main' işlevi içeriyor. Program yürütme orada başlayıp biter.
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>

using namespace std;
using namespace cv;
int main()
{
	string im_path = "D:/Peresthayal/WorkStation/Projects/_src/lena.jpg";
   
	Mat image = imread(im_path, IMREAD_COLOR);
	imshow("Original Image", image);
	
	Mat mask;
	inRange(image, Scalar(170, 20, 170), Scalar(255, 255, 255), mask);
	imshow("Mask", mask);

	Mat maskedImage;
	image.copyTo(maskedImage, mask);
	imshow("Masked Image", maskedImage);

	cout << "Mask Channel Length:" << mask.channels() << endl;

	Mat mask3d;
	cvtColor(mask, mask3d, CV_GRAY2BGR);

	Mat maskedImageAnd = image & mask3d;
	imshow("3d Mask &", maskedImageAnd);
	
	Mat inverseMask3d = ~mask3d;
	Mat inverseMasked3d = image & inverseMask3d;
	imshow("Inverse Masked 3d &", inverseMasked3d);

	/* OpenCV XML ve YAML İşlemleri*/
	//FileStorage fs;
	//fs.open("parameter.xml", FileStorage::WRITE);

	FileStorage fs("parameter.xml", FileStorage::WRITE);
	fs << "ImageData" << mask;
	fs.release();
	
	

	FileStorage fs_read("parameter.xml", FileStorage::READ);
	Mat bufferedImage;
	
	fs_read["ImageData"] >> bufferedImage;
	imshow("Buffered Image", bufferedImage);
	cout << bufferedImage;
	waitKey(0);
}

