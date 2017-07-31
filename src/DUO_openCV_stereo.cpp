///////////////////////////////////////////////////////////////////////////////////
// This code sample demonstrates the use of DUO SDK in your own applications
// For updates and file downloads go to: http://duo3d.com/
// Copyright 2014 (c) Code Laboratories, Inc.  All rights reserved.
///////////////////////////////////////////////////////////////////////////////////

#include "DUO_openCV_stereo.hpp"

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#define WIDTH	640
#define HEIGHT	480
#define FPS		30
using namespace std;

void erodeANDdelate(Mat &imagineProcesata)
{
	Mat erodeElement = getStructuringElement(MORPH_ELLIPSE,Size(3,3));
	Mat delateElement = getStructuringElement(MORPH_ELLIPSE,Size(3,3));

	dilate(imagineProcesata,imagineProcesata,delateElement);
	dilate(imagineProcesata,imagineProcesata,delateElement);

	erode(imagineProcesata,imagineProcesata,erodeElement);
	erode(imagineProcesata,imagineProcesata,erodeElement);
}

void getStereo(char *windowDisparity, const Mat& imgLeft, const Mat& imgRight, int disparity, int SADWindowSize){
	// Create the image in which we will save our disparities
	Mat imgDisparity16S = Mat( imgLeft.rows, imgLeft.cols, CV_16S );
	Mat imgDisparity8U = Mat( imgLeft.rows, imgLeft.cols, CV_8UC1 );

	//-- 2. Call the constructor for StereoBM
	int ndisparities = 16*disparity;
	StereoBM sbm( StereoBM::FISH_EYE_PRESET, ndisparities, SADWindowSize );

	//-- 3. Calculate the disparity image
	sbm( imgLeft, imgRight, imgDisparity16S, CV_16S );

	//-- Check its extreme values
	double minVal; double maxVal;
	minMaxLoc( imgDisparity16S, &minVal, &maxVal );

	//-- 4. Display it as a CV_8UC1 image
	imgDisparity16S.convertTo( imgDisparity8U, CV_8UC1, 255/(maxVal - minVal));
	namedWindow( windowDisparity, WINDOW_NORMAL );
	imshow( windowDisparity, imgDisparity8U );
}

void detectAndShowHumanDefault(Mat img){//char* name){
	HOGDescriptor hog;//(Size(64, 128), Size(16, 16), Size(8, 8), Size(8, 8), 9, DEFAULT_WIN_SIGMA, 1.2, true, HOGDescriptor::DEFAULT_NLEVELS);
	unsigned i;
	hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
	vector<Rect> found;
	vector<Point> foundLocations;
	hog.detectMultiScale(img, found, 0, Size(8,8), Size(500,500), 1.05, 2);

	cout<<"found: "<<found.size()<<" people"<<endl;
	for(i=0; i<found.size(); i++) {
		cout << "widht = " << found[i].width<<" height = " << found[i].height << endl;
		rectangle(img,Point(found[i].x,found[i].y),Point(found[i].x+found[i].width,found[i].y+found[i].height),Scalar(255,0,0),1,8,0);
	}
}

int main(int argc, char* argv[])
{
	printf("DUOLib Version:       v%s\n", GetLibVersion());

	// Open DUO camera and start capturing
	if(!OpenDUOCamera(WIDTH, HEIGHT, FPS))
	{
		printf("Could not open DUO camera\n");
		return 0;
	}
	// Set exposure and LED brightness
	SetExposure(100);
	SetLed(35);
	SetDUOVFlip(_duo, true);

	// Create image headers for left & right frames
	Mat left, right, leftRGB, rightRGB;
	int SADWindowSize = 19;
	int disparity = 2;
	int key = 0;

	// Run capture loop until <Esc> key is pressed
	while((key & 0xff) != 27)
	{
		// Capture DUO frame
		PDUOFrame pFrameData = GetDUOFrame();
		if(pFrameData == NULL) continue;

		// Set the image data
		left = Mat(Size(pFrameData->width, pFrameData->height), CV_8UC1, pFrameData->leftData);
		right = Mat(Size(pFrameData->width, pFrameData->height), CV_8UC1, pFrameData->rightData);
//		cvtColor(left, leftRGB, COLOR_GRAY2BGR);
//		cvtColor(right, rightRGB, COLOR_GRAY2BGR);

		// Process images here (optional)
		//cout<<"image width = "<<left.cols<<" image height = " << left.rows<<endl;
		getStereo("Disparity", left, right, disparity, SADWindowSize);
		// Display images
		imshow("left", left);
		imshow("right", right);

		//leftRGB = Mat(left);
		//detectAndShowHumanDefault(leftRGB);

		key = cvWaitKey(10);
//		if (key & 0xff != -1) {
//			switch (key & 0xff) {
//			case 113://q
//				disparity ++;
//				cout<<"disparity = "<<disparity<<endl;
//				break;
//			case 97://a
//				if (disparity > 0)
//					disparity --;
//				cout<<"disparity = "<<disparity<<endl;
//				break;
//			case 112://p
//				SADWindowSize += 2;
//				cout << "SAD = "<<SADWindowSize<<endl;
//				break;
//			case 108://l
//				if (SADWindowSize >= 7)
//					SADWindowSize -= 2;
//				cout << "SAD = "<<SADWindowSize<<endl;
//				break;
//			}
//		}
	}
	// Close DUO camera
	CloseDUOCamera();
	return 0;
}

