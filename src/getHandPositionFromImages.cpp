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
    unsigned int i;
    char denumiri[8][27] = {"images/inputData/hand1.jpg","images/inputData/hand2.jpg",
            "images/inputData/hand3.jpg","images/inputData/hand4.jpg","images/inputData/hand5.jpg",
            "images/inputData/hand6.jpg","images/inputData/hand7.jpg","images/inputData/hand7.jpg"};

    if(!open_hand_cascade.load("openhand-haar.xml")) {
        cout<<"error loading open hand \n";
        return 0;
    } else
        cout<<"hand loaded \n";

    for(i = 0; i < 8 ; i++) {
        img = imread(denumiri[i], CV_LOAD_IMAGE_GRAYSCALE);
        hand = detectHand(img);
        if(!hand.empty()) {
            cout <<"x= " << currentPosition.x << "; y= " << currentPosition.y ;
            cout << "; rezolutie: " << img.rows << "x" << img.cols << endl;
        }
        imshow("img",img);
        waitKey(1000);
    }//end while sau for
    cout<<"exit..."<<endl;
    return 0;
}
