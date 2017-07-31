#include <cstdlib>
#include <vector>
#include <iostream>

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace cv;
using namespace std;


//global variable
CascadeClassifier open_hand_cascade;
struct Position {
    int x;
    int y;
};
Position currentPosition;
///////

Mat detectHand(Mat image) {
	Mat hand;
	std::vector<Rect> hands;

	open_hand_cascade.detectMultiScale(image,hands,1.1,3,CV_HAAR_FIND_BIGGEST_OBJECT,Size(30,30),Size(500,500));

	if(hands.size())
	{
		hand = image(hands[0]);
		//rectangle(image,Point(hands[0].x,hands[0].y),Point(hands[0].x+hands[0].width,hands[0].y+hands[0].height),Scalar(255,0,0),1,8,0);
        currentPosition.x = hands[0].x + (hands[0].width/2);
        currentPosition.y = hands[0].y + (hands[0].height/2);
	}
	return hand;
}

int main(void)
{
	Mat imageOriginal, hand, img;
	int key = -1;
    VideoCapture cap;
	
    for (int i=0; i<10; i++) {
    	cap = VideoCapture(i);
		if(!cap.isOpened()) {
			cout<<"nu s-a deschis camera " << i << endl;
			continue;
		} else {
			cout<<"camera " << i << " s-a deschis " << endl;
			key = i;
			break;
		}
    }

    if (key == -1) {
    	return 0;
    }

	if(!open_hand_cascade.load("openhand-haar.xml")) {
		cout<<"eror loading open hand \n";
		return 0;
	} else
		cout<<"hand loaded \n";

	for(key = 0; key < 200; key++) {
		cap>>img;
        hand = detectHand(img);
		if(!hand.empty()) {
            cout << "(" << key << ") x= " << currentPosition.x << "; y= " << currentPosition.y ;
            cout << "; rezolutie: " << img.rows << "x" << img.cols << endl;
		}
	}//end for
    cout<<"exit..."<<endl;
	return 0;
}
