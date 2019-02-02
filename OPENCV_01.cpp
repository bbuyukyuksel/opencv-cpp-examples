// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include <stdio.h>

#include <string>

using namespace std;
using namespace cv;

void ImageReadWrite(string);
void WebcamPlayer(void);

int main()
{
	string im_path = "D:/Peresthayal/WorkStation/Projects/_src/lena.jpg";
	ImageReadWrite(im_path);
	printf("%s", "Burak Buyukyuksel!\n");
	cv::waitKey();
	WebcamPlayer();

	
}


void ImageReadWrite(string path) {
	string winName = "LENA_ORG";
	cv::Mat Image = cv::imread(path, cv::IMREAD_COLOR);
	if (Image.data) {
		cv::namedWindow(winName);
		cv::imshow(winName, Image);
		cout << "Image is read!" << endl;
		cv::waitKey();

		cv::imwrite("lena.png", Image);
		
		vector<int> pJpg;
		pJpg.push_back(CV_IMWRITE_JPEG_QUALITY);
		pJpg.push_back(30);
		cv::imwrite("ebru.jpeg", Image, pJpg);
		
		cv::Mat pngLena = cv::imread("lena.png", cv::IMREAD_COLOR);
		cv::imshow("Result: PNG", pngLena);
		cout << ".PNG Image is read!" << endl;
		cv::waitKey(0);
		
		cv::Mat jpegLena = cv::imread("lena.jpeg", cv::IMREAD_COLOR);
		cv::imshow("Result: JPEG", jpegLena);

		cout << ".JPEG Image is read!" << endl;
		cv::waitKey(0);
		
		cv::destroyAllWindows();
	}
	else {
		cout << "Image could not open!";
	}
}

void WebcamPlayer(void) {
	cout << "Webcam player" << endl;
	string wName = "Webcam Player";
	int fps_count = 0;
	cv::VideoCapture vCap(0);
	if (vCap.isOpened()) {
		double fps = vCap.get(CV_CAP_PROP_FPS);
		if (fps < 1) {
			// It coult not take always correctly fps information
			fps = 23;
			cout << "FPS: " << fps << endl;
		}
		cv::Mat frame;
		cv::namedWindow(wName);
		while (true) {
			if (vCap.read(frame))
				cv::imshow(wName, frame);
			fps_count++;
			if (cv::waitKey(5000 / fps)) break;
		}
		vCap.release();
		cv::destroyWindow(wName);
		cout << "Frame Count: " << fps_count << endl;
		cv::waitKey();
	}
	else {
		cout << "Web cam is not opened!" << endl;
	}
}
