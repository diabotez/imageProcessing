#include <cstdlib>
#include <vector>
#include <iostream>

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace cv;
using namespace std;

//global variable
struct Position {
	int x;
	int y;
	int width;
	int height;
};
Position currentPosition;
CascadeClassifier open_hand_cascade;

Mat searchForBiggestHand(Mat image) {
	Mat hand;
	std::vector<Rect> hands;

	open_hand_cascade.detectMultiScale(image, hands, 1.1, 3,
	CV_HAAR_FIND_BIGGEST_OBJECT, Size(30, 30), Size(500, 500));

	if (hands.size()) {
		hand = image(hands[0]);
		//rectangle(image,Point(hands[0].x,hands[0].y),Point(hands[0].x+hands[0].width,hands[0].y+hands[0].height),Scalar(255,0,0),1,8,0);
		currentPosition.x = hands[0].x; //
		currentPosition.width = hands[0].width;
		currentPosition.y = hands[0].y;
		currentPosition.height = hands[0].height;
	}
	return hand;
}

int searchHandInVideoMain(void) {
	Mat imageOriginal, hand, img;
	int key = -1;
	VideoCapture cap;
	namedWindow("video", cv::WINDOW_AUTOSIZE);

	for (int i = 0; i < 10; i++) {
		cap = VideoCapture(i);
		if (!cap.isOpened()) {
			cout << "nu s-a deschis camera " << i << endl;
			continue;
		} else {
			cout << "camera " << i << " s-a deschis " << endl;
			key = i;
			break;
		}
	}

	if (key == -1) {
		return 0;
	}

	if (!open_hand_cascade.load("openhand-haar.xml")) {
		cout << "eror loading open hand \n";
		return 0;
	} else
		cout << "hand loaded \n";

	while (key != 27) {
		cap >> img;
		hand = searchForBiggestHand(img);
		if (!hand.empty()) {
			imshow("hand", hand);
			rectangle(img, Point(currentPosition.x, currentPosition.y),
					Point(currentPosition.height, currentPosition.width),
					Scalar(255, 0, 0), 1, 8, 0);
		}

		imshow("video", img);
		key = waitKey(10);
	} //end for

	cap.release();
	cout << "exit..." << endl;
	return 0;
}

Mat enhanceDetails(Mat img) {
	Mat result, originalFilteredImg, equalizedImg;
	float f = 0.5;

	float data1[] = { (-f) / 256, (-4 * f) / 256, (-6 * f) / 256, (-4 * f)
			/ 256, (-f) / 256, (-4 * f) / 256, (-16 * f) / 256, (-24 * f) / 256,
			(-16 * f) / 256, (-4 * f) / 256, (-16 * f) / 256, (-24 * f) / 256,
			(220 * f) / 256, (-24 * f) / 256, (-16 * f) / 256, (-4 * f) / 256,
			(-16 * f) / 256, (-24 * f) / 256, (-16 * f) / 256, (-4 * f) / 256,
			(-f) / 256, (-4 * f) / 256, (-6 * f) / 256, (-4 * f) / 256, (-f)
					/ 256 };
	Mat kernel1 = Mat(5, 5, CV_32FC1, &data1);

//	float data2[] = { -f / 16, (-2 * f) / 16, -f / 16, (-2 * f) / 16, (12 * f)
//			/ 16, (-2 * f) / 16, -f / 16, (-2 * f) / 16, -f / 16 };
	float data2[] = { -f / 16, (-f) / 16, -f / 16, (-f) / 16, (8 * f) / 16, (-f)
			/ 16, -f / 16, (-f) / 16, -f / 16 };
	Mat kernel2 = Mat(3, 3, CV_32FC1, &data2);

	imshow("original", img);
	equalizeHist(img, equalizedImg);
	imshow("equalized without filter", equalizedImg);

	filter2D(img, originalFilteredImg, CV_32FC1, kernel1);
	imshow("originalFilteredImg", originalFilteredImg);

	addWeighted(equalizedImg, 1.0 - f, originalFilteredImg, 1, 0.0, result, 0);
	imshow("equalizedImg + originalFilteredImg", result);
	equalizeHist(result, result);
	imshow("enhanced equalizedImg + originalFilteredImg", result);

	return result;
}

void derivativesOfAnImage(Mat img) {
	Mat xFirstDerivate, yFirstDerivate;
	Mat xySecondDerivate, xsecondDerivate;

	Mat result, originalFilteredImg, equalizedImg;
	float f = 1.0;

	float data2[] = { -f, 0.0, f, -f, 0.0, f, -f, 0.0, f };
	float yData[] = { f, f, f, 0.0, 0.0, 0.0, -f, -f, -f };
	Mat kernel1 = Mat(3, 3, CV_32FC1, &data2);
	Mat yKernel = Mat(3, 3, CV_32FC1, &yData);

	imshow("original", img);

	filter2D(img, xFirstDerivate, CV_32FC1, kernel1);
	filter2D(img, yFirstDerivate, CV_32FC1, yKernel);
	imshow("xFirstDerivate", xFirstDerivate);
	imshow("yFirstDerivate", yFirstDerivate);

	filter2D(xFirstDerivate, xsecondDerivate,CV_32FC1, kernel1);
	imshow("xsecondDerivate", xsecondDerivate);

	filter2D(xFirstDerivate, xySecondDerivate, CV_32FC1, yKernel);
	imshow("xySecondDerivate", xySecondDerivate);
}

void gradientOfAnImage(Mat img) {
	Mat xDerivate, yDerivate, gradient;
	float f = 1.0;

	float xData[] = { -f, 0.0, f, -f, 0.0, f, -f, 0.0, f };
	float yData[] = { f, f, f, 0.0, 0.0, 0.0, -f, -f, -f };
	Mat xKernel = Mat(3, 3, CV_32FC1, &xData);
	Mat yKernel = Mat(3, 3, CV_32FC1, &yData);

	imshow("original", img);

	filter2D(img, xDerivate, CV_32FC1, xKernel);
	filter2D(img, yDerivate, CV_32FC1, yKernel);

	addWeighted(xDerivate, 2.0, yDerivate, 2.0, 0.0, gradient);

	divide(1.0/2, gradient, gradient);


	imshow("gradient", gradient);
}

int main() {
	Mat img = imread("1.png", IMREAD_GRAYSCALE); // IMREAD_GRAYSCALE
//	resize(img, img, Size(640,480));

	gradientOfAnImage(img);

	waitKey(0);
}

