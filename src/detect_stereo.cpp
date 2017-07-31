#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "opencv2/video/tracking.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"

using namespace std;
using namespace cv;

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
    erodeANDdelate (imgDisparity8U);
    namedWindow( windowDisparity, WINDOW_NORMAL );
    imshow( windowDisparity, imgDisparity8U );
}

int main(int argc, char** argv) {
    Mat imgLeft = imread("images/stereo2D/img11.png", CV_LOAD_IMAGE_GRAYSCALE);
    Mat imgRight = imread("images/stereo2D/img12.png", CV_LOAD_IMAGE_GRAYSCALE);
 
    if (!imgLeft.empty() && !imgRight.empty()) {;
        getStereo("Disparity 2-11", imgLeft, imgRight, 2, 11);
        getStereo("Disparity 2-13", imgLeft, imgRight, 2, 13);
        getStereo("Disparity 2-15", imgLeft, imgRight, 2, 15);
        getStereo("Disparity 2-17", imgLeft, imgRight, 2, 17);
        getStereo("Disparity 2-19", imgLeft, imgRight, 2, 19);
        getStereo("Disparity 2-21", imgLeft, imgRight, 2, 21);
        getStereo("Disparity 2-23", imgLeft, imgRight, 2, 23);
        getStereo("Disparity 2-25", imgLeft, imgRight, 2, 25);
        getStereo("Disparity 2-27", imgLeft, imgRight, 2, 27);
        getStereo("Disparity 2-29", imgLeft, imgRight, 2, 29);
        waitKey();
    }

    return 0;
}
