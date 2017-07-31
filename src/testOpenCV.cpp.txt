
#include <cstdlib>
#include <cstdio>
#include <iostream>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"

using namespace cv;
using namespace std;

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

int main( int argc, char** argv )
{
	Mat src = imread( argv[1], 1 );

	cvtColor( src, src, CV_BGR2GRAY );
	cout << "imagine convertita in grayscale" << endl;
	detectAndShowHumanDefault(src);
	imwrite("grayHumanDetect",src);

	return 0;
}
