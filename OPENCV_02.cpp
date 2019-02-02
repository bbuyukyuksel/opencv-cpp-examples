// OPENCV_02.cpp : This file contains the 'main' function. Program execution begins and ends there.
//



#include "pch.h"
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>


using namespace std;
using namespace cv;

void MatDemo(void);
void Drawing(int);
void* pixelProcess(void);

string im_path = "D:/Peresthayal/WorkStation/Projects/_src/lena.jpg";

struct splitedImage
{
	Mat r;
	Mat g;
	Mat b;
	double time;
};

#define version 1

int main()
{

	//Mat::Mat(int rows, int cols, int type);
	//Mat::create(int rows, int cols, int type);

	// 120 x 160 x 1
	//Mat image(120, 160, CV_8UC1);

	// 480 x 640 x 3
	//Mat img;
	//img.create(480, 640, CV_8UC3);


#if version == 1
	MatDemo();
#elif version == 2
	Drawing(3);
#elif version == 3
	struct splitedImage *mySplitedImage=  (struct splitedImage*)pixelProcess();
	Mat r = mySplitedImage->r;
	Mat g = mySplitedImage->g;
	Mat b = mySplitedImage->b;
	imshow("R:", r);
	imshow("G:", g);
	imshow("B:", b);
	cout << "Time: " << mySplitedImage->time;

	Mat src = imread(im_path, IMREAD_COLOR);
	Mat bgr[3];
	split(src, bgr);
	imshow("BLUE", bgr[0]);
	imshow("GREEN", bgr[1]);
	imshow("RED", bgr[2]);
#endif
	waitKey();
}

void MatDemo() {
	
	cout << "DEMO" << endl;
	string wName = "DEMO";
	namedWindow(wName);
	Mat image(480, 640, CV_8UC3);
	randu(image, Scalar(0, 0, 0), Scalar(255, 255, 255));
	imshow(wName, image);
	waitKey(500);
	
	// BGR
	Mat red_roi(image, Rect(40, 20, image.cols - 80, image.rows - 40));
	red_roi = Scalar(0, 0, 255);
	Mat cp_red_roi = red_roi.clone();
	 
	Mat green_roi(red_roi, Rect(40, 20, red_roi.cols - 80, red_roi.rows - 40));
	green_roi = Scalar(0, 255, 0);
	Mat cp_green_roi = green_roi.clone();

	Mat blue_roi(green_roi, Rect(40, 20, green_roi.cols - 80, green_roi.rows - 40));
	blue_roi = Scalar(255, 0, 0);
	Mat cp_blue_roi = blue_roi.clone();

	image.rowRange(125, 165).setTo(Scalar(255, 255, 0));
	image.colRange(195, 240).setTo(Scalar(0, 255, 255));
	//Copy roi to another variable
	//Mat test = image.rowRange(125, 165).clone();
	//imshow("TEST", test);

	imshow("ORG", image);
	imshow("RED   ROI", red_roi);
	imshow("GREEN ROI", green_roi);
	imshow("BLUE  ROI", blue_roi);
	waitKey();

	imshow("CP RED", cp_red_roi);
	imshow("CP GREEN", cp_green_roi);
	imshow("CP BLUE", cp_blue_roi);
	waitKey();
}

void Drawing(int cycle) {
	
	cout << "Drawing Demo" << endl;
	string wName = "Drawing Demo";
	Mat bImage = imread(im_path, IMREAD_COLOR);
	Mat cp_bImage;
	bImage.copyTo(cp_bImage);

	int nCols = bImage.cols;
	int nRows = bImage.rows;
	int hour = -1;
	for (int i = 0 - 90; i <= (360 - 90); i++)
	{
		if (i % 30 == 0) {
			hour++;
			putText(bImage, to_string(hour % 12), Point(nCols / 2 + 120 * cos(i* CV_PI / 180) - 10, nRows / 2 + 120 * sin(i* CV_PI / 180) + 15), FONT_HERSHEY_SCRIPT_SIMPLEX | FONT_ITALIC, 1, Scalar(255, 255, 255), 1);
			
		}
	}
	hour = 0;
	for (int i = 0 - 90 - 1; i <= cycle * (360) - 90 - 1; i++) {
		line(cp_bImage,
			Point(nCols / 2, nRows / 2),
			Point(nCols / 2 + 100 * cos(i* CV_PI / 180), nRows / 2 + 100* sin(i* CV_PI / 180)),
			Scalar(0, 0, 255),
			3
		);
		putText(cp_bImage,"#", Point(nCols / 2 + 100 * cos(i* CV_PI / 180), nRows / 2 + 100 * sin(i* CV_PI / 180)), FONT_HERSHEY_SCRIPT_SIMPLEX | FONT_ITALIC, 0.4, Scalar(255, 255, 255), 1,1);

		if (i % 30 == 0) {
			hour++;
			waitKey(100);
		}

		putText(cp_bImage, wName, Point(i + 10, i + 10), FONT_HERSHEY_COMPLEX_SMALL | FONT_ITALIC, 2, Scalar(255, 0, 0), 3);
		circle(cp_bImage, Point(nCols - 1.3 * i, 400), 15, Scalar(0, 255, 255), -1);
		rectangle(cp_bImage, Rect(0 + 1.3*i, 380 - 0.9 *i, 40, 40), Scalar(255, 255, 0), 8);
		imshow(wName, cp_bImage);
		waitKey(30);
		bImage.copyTo(cp_bImage);
	}
}

void* pixelProcess(void) {
	
	struct splitedImage *mySplitedImage = new struct splitedImage;

	// Without use iterator.
	// image.at<uchar>)(x,y)
	// image.at<cv::Vec3b>(x,y)[0]
	// image.at<cv::Vec3b>(x,y)[1]
	// image.at<cv::Vec3b>(x,y)[2]

	// Using with iterator
	double tStart, tEnd;

	Mat image = imread(im_path, IMREAD_COLOR);
	cout << "Image Channels: " << image.channels() << endl;

	Mat r_image(image.rows, image.cols, CV_8UC1);
	Mat g_image(image.rows, image.cols, CV_8UC1);
	Mat b_image(image.rows, image.cols, CV_8UC1);

	MatIterator_ <cv::Vec3b> it;
	MatIterator_ <uchar> r_it;
	MatIterator_ <uchar> g_it;
	MatIterator_ <uchar> b_it;	
	

	r_it = r_image.begin<uchar>();
	g_it = g_image.begin<uchar>();
	b_it = b_image.begin<uchar>();

	tStart = (double)getTickCount();
	for (it = image.begin<cv::Vec3b>(); it != image.end<Vec3b>(); it++) {
		uchar blue = (*it)[0];
		uchar green = (*it)[1];
		uchar red = (*it)[2];
		(*r_it++) = red;
		(*g_it++) = green;
		(*b_it++) = blue;
	}
	tEnd = (double)getTickCount();
	
	mySplitedImage->r = r_image;
	mySplitedImage->g = g_image;
	mySplitedImage->b = b_image;
	mySplitedImage->time = double(tEnd - tStart) / getTickFrequency();

	return (void*)mySplitedImage;
	
}