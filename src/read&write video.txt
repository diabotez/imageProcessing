#include <cstdlib>
#include <vector>
#include <iostream>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
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
		rectangle(image,Point(hands[0].x,hands[0].y),Point(hands[0].x+hands[0].width,hands[0].y+hands[0].height),Scalar(255,0,0),1,8,0);
        currentPosition.x = hands[0].x + (hands[0].width/2);
        currentPosition.y = hands[0].y + (hands[0].height/2);
	}

	/*
	for(int i=0; i<hands.size(); i++)
	{
	std::cout<<"hands[i].x = "<<hands[i].x<<"\n";
	std::cout<<"hands[i].y = "<<hands[i].y<<"\n";
	
	}
	String s = "hands\hand";
	j++;
	std::ostringstream convert;
	std::string k_string;
	convert << j;
	k_string = convert.str();
	s+=k_string;
	s+=".jpg";
	imwrite(s,hand);
*/	
    //namedWindow("open_hand",1);
	//imshow("open_hand",hand);

	return hand;
}

int main(void)
{
	Mat imageOriginal, hand, img;
	char key = 0;
    VideoCapture cap = VideoCapture(1);//"handVideo.avi");//"inputData/movie2.mov"
	//char denumiri[7][23] = {"inputData/DSC00273.JPG","inputData/DSC00274.JPG","inputData/DSC00275.JPG","inputData/DSC00278.JPG","inputData/DSC00279.JPG","inputData/DSC00282.JPG","inputData/DSC00283.JPG"};
    //int cnt = 7;
  
    //VideoWriter outputVideo;
    //int ex = static_cast<int>(cap.get(CV_CAP_PROP_FOURCC));
    //Size S = Size((int) cap.get(CV_CAP_PROP_FRAME_WIDTH),    // Acquire input size
    //              (int) cap.get(CV_CAP_PROP_FRAME_HEIGHT));

	if(!cap.isOpened()) {
		std::cout<<"nu s-a deschis camera \n";
		return 0;
	} else
		std::cout<<"camera s-a deschis \n";
	if(!open_hand_cascade.load("openhand-haar.xml")) {
		std::cout<<"eror loading open hand \n";
		return 0;
	} else
		std::cout<<"hand loaded \n";

    //outputVideo.open("handVideo2.avi", ex=-1, (cap.get(CV_CAP_PROP_FPS))/2, S, true);
    //if (!outputVideo.isOpened())
    //{
    //    cout  << "Could not open the output video for write " << endl;
    //    return -1;
    //}
   // cap>>imageOriginal;
    //for (int i=0; i<cnt; i++) {
	while(key != 27) {
		cap>>img;//imageOriginal;
        //if(imageOriginal.empty()) break;
        //imageOriginal = imread(denumiri[i],1);
        //resize(imageOriginal,img,Size(640,480),0.0,0.0,INTER_CUBIC);
		
        hand = detectHand(img);
		if(!hand.empty()) {
			imshow("hand" , hand);
            cout << "x= " << currentPosition.x << "; y= " << currentPosition.y ;
            cout << "; rezolutie: " << img.rows << "x" << img.cols << endl;
		}

		imshow("original",img);
        //outputVideo.write(img);
		key = waitKey(15);
		//key = waitKey();
	}//end while sau for
	return 0;
}
