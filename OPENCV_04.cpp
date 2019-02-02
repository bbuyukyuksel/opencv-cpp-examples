// OPENCV_04.cpp : Bu dosya 'main' işlevi içeriyor. Program yürütme orada başlayıp biter.
//

#include "pch.h"
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>

using namespace cv;
using namespace std;

// Versiyon Seçimini Buradan Yapınız.
#define version 1


/*
*
*	ÖN TANIMLAMALAR
*
*/
#if version == 1
void MorpholojiIntroDemo();
void trackBarFunction(int value, void* userData);

struct trackBarParams {
	int *opCode;
	int *iteration;
	Mat tImg;
	string wName;
};

#elif version == 2
void BlurMethods(void);
void mouse_callback(int  event, int  x, int  y, int  flag, void *param);
Point oldPoint = Point(-1, -1);
Point newPoint = Point(-1, -1);
bool flag4Drawing = false;
bool eventMode = false;

#elif version == 3
void EdgeDetections(void);
#elif version == 4
void CoinCount(void);


#endif



int main()
{
#if version == 1
	MorpholojiIntroDemo();
#elif version == 2
	BlurMethods();
#elif version == 3
	EdgeDetections();
#elif version == 4
	CoinCount();
#endif
	waitKey(0);
}

#if version == 1
void MorpholojiIntroDemo() {
	cout << "Intro" << endl;
	string im_path = "D:/Peresthayal/WorkStation/Projects/_src/lena640x480.jpg";
	int iteration = 0, iteration_Max = 5;
	int opCode = 0, opCode_Max = 6;

	Mat cImg = imread(im_path, IMREAD_COLOR);
	Mat zImg = Mat::zeros(640, 880, CV_8UC3);
	Mat roi(zImg, Rect(100, 100, cImg.cols, cImg.rows));
	cImg.copyTo(roi);

	//resize(cImg, cImg, Size(640, 480), 0, 0, INTER_CUBIC);
	//imwrite("D:/Peresthayal/WorkStation/Projects/_src/lena640x480.jpg",cImg);

	string wName = "Morpholoji Demo";
	string wSetName = "Settings Page";
	namedWindow(wName);
	namedWindow(wSetName);

	imshow(wName, zImg);

	trackBarParams params;
	params.iteration = &iteration;
	params.opCode = &opCode;
	params.tImg = zImg;
	params.wName = wName;


	createTrackbar("Morph Mode", wSetName, &opCode, opCode_Max, trackBarFunction, &params);
	createTrackbar("Iteration", wSetName, &iteration, iteration_Max, trackBarFunction, &params);

	setTrackbarPos("Morph Mode", wSetName, 0);
	setTrackbarPos("Iteration", wSetName, 0);

	waitKey(0);
	destroyWindow(wName);

	/*
	imshow("ROI", roi);
	imshow("ORG", cImg);
	cImg.copyTo(roi);
	*/
	waitKey(0);
}
void trackBarFunction(int value, void* userData) {
	struct trackBarParams params = *((struct trackBarParams*)userData);
	int * iteration = params.iteration;
	int * opCode = params.opCode;
	Mat cImg = params.tImg;
	Mat tImg = cImg.clone();
	Mat dst;
	string wName = params.wName;

	cout << *iteration << "  :   " << *opCode << endl;
	const char* morphNames[] = { "Erosion",
		"Dilation",
		"Open",
		"Close",
		"Gradient",
		"Top Hat",
		"Black Hat"
	};
	int opTypes[] = { MORPH_ERODE,
					  MORPH_DILATE,
					  MORPH_OPEN,
					  MORPH_CLOSE,
					  MORPH_GRADIENT,
					  MORPH_TOPHAT,
					  MORPH_BLACKHAT };


	putText(tImg, morphNames[*opCode], Point(10, 45), FONT_HERSHEY_COMPLEX_SMALL | FONT_ITALIC, 3, Scalar(255, 255, 255), 2);
	Mat SE = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	if ((*iteration) != 0) {
		morphologyEx(tImg, dst, opTypes[*opCode], SE, Point(-1, -1), *iteration);
		imshow(wName, dst);
	}
	else {
		imshow(wName, tImg);
	}
}
#elif version == 2
void BlurMethods(void) {
	string im_path = "D:/Peresthayal/WorkStation/Projects/_src/coins.jpg";
	Mat I = imread(im_path, IMREAD_COLOR);
	Size kernel_size; // Size(width, height);

	if (I.data == NULL) {
		cout << "Image could not READ!" << endl;
		return;
	}
	if (I.channels() > 1) {
		cvtColor(I, I, CV_BGR2GRAY);
	}

	// Blur Image 
	Mat blurImage;
	kernel_size = Size(11, 3);
	blur(I, blurImage, kernel_size);
	string wName4Blur = "Blur Image";

	// Gaussian Blur
	Mat gaussianImage;
	kernel_size = Size(11,11);
	int sigma_x = 3;
	int sigma_y = 3;
	GaussianBlur(I, gaussianImage, kernel_size, sigma_x, sigma_y);
	string wName4Gaussian = "Gaussian Image";

	Mat bilareralImage;
	int diaOfNeighbors = -1; //If this assignment is negative value, it will calculate using sigmaSpace value.
	int sigmaColor = 50; //Color space distance measure
	int sigmaSpace = 9; //Coord System distance measure, if diaOfNeighbor < 0, Calculate distance measure using sigmaSpace.
	bilateralFilter(I, bilareralImage, diaOfNeighbors, sigmaColor, sigmaSpace);
	cvtColor(bilareralImage, bilareralImage, CV_GRAY2BGR);
	string wName4Bilateral = "Bilateral";

	namedWindow(wName4Blur);
	namedWindow(wName4Gaussian);
	namedWindow(wName4Bilateral);

	
	setMouseCallback(wName4Bilateral, mouse_callback, &bilareralImage);

	imshow("Original Image", I);
	imshow(wName4Blur, blurImage);
	imshow(wName4Gaussian, gaussianImage);
	imshow(wName4Bilateral, bilareralImage);

	cout << "If you want to draw, you can use Bilateral Window" << endl;
	cout << "Use [right click] to change drawing type " << endl;
	cout << "\t [ctrl + q] continues mode" << endl;
	cout << "\t [ctrl + w] disctrete mode" << endl;

	waitKey(0);
}
void mouse_callback(int  event, int  x, int  y, int  flag, void *param)
{
	Mat I = *((Mat*)param);


	/*
	if (event == EVENT_MOUSEMOVE) {
		// Its run always
	}
	*/
	if (event == EVENT_LBUTTONDOWN) {
		if (oldPoint.x == -1 || oldPoint.y == -1) {
			oldPoint.x = x;
			oldPoint.y = y;
			return;
		}
		newPoint.x = x;
		newPoint.y = y;

		flag4Drawing ^= true;


		if (!eventMode) {
			if (flag4Drawing) {
				//cout << "Start Point:" << oldPoint << " | Finish Point:" << newPoint << "Flag Status:" << flag4Drawing << endl;
				line(I, oldPoint, newPoint, Scalar(255, 0, 0));
				imshow("Bilateral", I);
			}
		}
		else {
			//cout <<"Event Mode:"<< eventMode <<" | Start Point:" << oldPoint << " | Finish Point:" << newPoint << "Flag Status:" << flag4Drawing << endl;
			line(I, oldPoint, newPoint, Scalar(0, 255, 0));
			imshow("Bilateral", I);
		}

		oldPoint = newPoint;

	}
	if (event == EVENT_RBUTTONDOWN) {
		cout << "Please Select Your Event" << endl;
		if (waitKey() == 23) {
			eventMode = false;
			oldPoint.x = -1;
			oldPoint.y = -1;
		}
		else if (waitKey() == 17) {
			eventMode = true;
			oldPoint.x = -1;
			oldPoint.y = -1;
		}
		string eventModeDesc = !eventMode ? "Discrete" : "Continues";
		cout << "Selected Event Mode:" << eventModeDesc << endl;
	}
}

#elif version == 3
void EdgeDetections(void) {
	string im_path = "D:/Peresthayal/WorkStation/Projects/_src/lena640x480.jpg";
	Mat I1 = imread(im_path, IMREAD_COLOR);
	Mat I;
	bilateralFilter(I1, I, -1, 50, 9);
	imshow("ORG", I);

	//Sobel
	Mat sobel;
	Sobel(I, sobel, -1, 2, 2, 5);

	// Laplacian
	Mat laplacian;
	Laplacian(I, laplacian, -1, 3);

	//Scharr
	Mat scharr;
	Scharr(I, scharr, -1, 0, 1);

	//Canny
	Mat canny;
	if (I.channels() > 1) {
		cvtColor(I, I, CV_BGR2GRAY);
	}
	Canny(I, canny, 0, 255);

	imshow("Sobel", sobel);
	imshow("Laplacian", laplacian);
	imshow("Scharr", scharr);
	imshow("Canny", canny);
	waitKey();
}
#elif version == 4
void CoinCount() {
	string im_path = "D:/Peresthayal/WorkStation/Projects/_src/coins_foreign.jpg";
	
	Mat oImg = imread(im_path, CV_LOAD_IMAGE_GRAYSCALE);
	String oImgWName = "Org Image";

	Mat tImg = oImg.clone();
	String tImgWName = "T Img";

	threshold(tImg, tImg, 200, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	Mat SE = getStructuringElement(MORPH_ELLIPSE, Size(3, 3), Point(-1, -1));
	Canny(tImg, tImg, 0, 255);
	imshow(tImgWName, tImg);
	

	//morphologyEx(tImg, tImg, MORPH_ERODE, SE, Point(-1, -1), 1);
	//morphologyEx(tImg, tImg, MORPH_CLOSE, SE, Point(-1, -1), 1);
	
	

	imwrite("bw_coins.jpg", tImg);

	namedWindow(oImgWName);
	namedWindow(tImgWName);

	imshow(oImgWName, oImg);
	imshow(tImgWName, tImg);

	waitKey(0);

	vector < vector <Point> > contours; // 2 Boyutlu Point değerlerini tutacak dizi oluşturuldu.
	vector <Point> contPoly;
	vector <Vec4i> hierarchy;

	/*
	*# Mode
	** CV_RETR_EXTERNAL	:	Sadece en dış sınır çizgilerini bulur.
	** CV_RETR_LIST		:	İç yada dış ayırt etmeksizin sınır çizgilerini listeler.	
	** CV_RETR_CCOMP	:	Hiyerarşi bilgisini sadece iç ve dış olarak tutar.
	** CV_RETR_TREE		:	Hiyerarşi bilgisini ağaç yapısında tutar.
	*
	*#Method
	** CV_CHAIN_APPROX_NONE									:	Bütün sınır noktalarını tutar.
	** CV_CHAIN_APPROX_SIMPLE								:	Yatay, dikey veya çarpraz sınır noktaları (sadece köşeleri tutar)
	** CV_CHAIN_APPROX_TC89_L1 | CV_CHAIN_APPROX_TC89_KCOS	:	Dominant nokta tespiti yöntemlerine göre çalışır.
	*/

	findContours(tImg, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	cout << "Number of coins :" << contours.size() << endl;
	waitKey(0);

	Mat outImg = Mat::zeros(tImg.size(), CV_8UC3);
	RotatedRect rRect;
	int countOfCoins = 0;
	for (int i = 0; i < contours.size(); i++) {
		
		rRect = minAreaRect(contours[i]);
		
		if (rRect.boundingRect().area() > 3000) {
			countOfCoins++;
			//cout << "Bounding Rect: " << rRect.boundingRect().area() << endl;
			putText(outImg, format("%d", i + 1), rRect.center, 1, 1, Scalar(0, 0, 255));
			drawContours(outImg, contours, i, Scalar(255, 0, 0), CV_FILLED, 8, hierarchy, 0, Point());
			imshow("Out Image", outImg);
			
			cout << "\rCout of coins :" << countOfCoins;
			waitKey(500);
		}
	}
	
	imshow("Out Image", outImg);
	waitKey(0);
}
#endif