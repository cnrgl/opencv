#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

const int max_H = 360 / 2;
const int max_value = 255;
const String window_capture = "Video Capture";
const String window_detect = "Object detection";
int low_H = 0, low_V = 0, low_S = 0;
int high_H=max_value, high_V=max_value, high_S=max_value;
//We are going to use HSV colorspace to change colorlevel threshold values
//Since the hue channel models the color type, it is very useful in image processing tasks that need to segment objects based on its color.

static void H_low_on_trackbar(int, void*)
{
	low_H = min(low_H,high_H-1);//check range side is whether not wrong
	setTrackbarPos("Low H", window_detect, low_H);
}
static void H_high_on_trackbar(int, void*)
{
	high_H = max(high_H,low_H+1);
	setTrackbarPos("High H",window_detect,high_H);
}
static void S_low_on_trackbar(int, void*)
{
	low_S = min(low_S, high_S - 1);
	setTrackbarPos("Low S", window_capture, low_S);
}
static void S_high_on_trackbar(int, void*)
{
	high_S = max(high_S, low_S + 1);
	setTrackbarPos("High S", window_detect, high_S);
}
static void V_low_on_trackbar(int,void *)
{
	low_V = min(low_V,high_V-1);
	setTrackbarPos("Low V",window_detect,low_V);
}
static void V_high_on_trackbar(int,void *)
{
	high_V = max(high_V,low_V+1);
	setTrackbarPos("High V",window_detect,high_V);
}

int main(int argc, char** argv)
{
	VideoCapture cap(argc > 1 ? argv[1] : "./sample.mp4");//Capture the video stream from default or supplied capturing device.
	if (!cap.isOpened()) {
		cout << "Error opening video stream or file" << endl;
		return -1;
	}
	//we will adjust the range of HVS colorspace color threshold range in order to extract desired color range.
	namedWindow(window_capture,WINDOW_AUTOSIZE);
	namedWindow(window_detect,WINDOW_AUTOSIZE);
	//Create the trackbars to set the range of HSV values
	createTrackbar("Low H",window_capture,&low_H,max_H,H_low_on_trackbar);
	createTrackbar("High H", window_capture, &high_H, max_H, H_high_on_trackbar);
	createTrackbar("Low S", window_capture, &low_S, max_value, S_low_on_trackbar);
	createTrackbar("High S", window_capture, &high_S, max_value, S_high_on_trackbar);
	createTrackbar("Low V",window_capture,&low_V,max_value,V_low_on_trackbar);
	createTrackbar("High V",window_capture,&high_V,max_value,V_high_on_trackbar);
	
	Mat frame, frame_HSV, frame_threshold;
	while(true)
	{ 
		cap >> frame;//frame store data from cap object which store instant object displaying...
		if(frame.empty())
		{
			break;
		}
		// Convert from BGR to HSV colorspace
		cvtColor(frame,frame_HSV,COLOR_BGR2HSV);
		// Detect the object based on HSV Range Values
		//the object filtering (like as binary!!! threshold ) according to range stated inRange function head
		inRange(frame_HSV, Scalar(low_H, low_S, low_V), Scalar(high_H,high_S,high_V), frame_threshold);
		/*
		cv::inRange	(	InputArray 	src,
		InputArray 	lowerb,
		InputArray 	upperb,
		OutputArray 	dst );

		The function checks the range as follows:
		dst(I)=lowerb(I)≤src(I)≤upperb(I)

		Parameters
		src-->	    first input array.
		lowerb-->	inclusive lower boundary array or a scalar.
		upperb-->	inclusive upper boundary array or a scalar.
		dst-->	    output array of the same size as src and CV_8U type.

*/
		//image frames showing
		imshow(window_capture, frame);
		imshow(window_detect,frame_threshold);
		
		char key = (char)waitKey(30);//~30 fps(frame per second) using
		if(key=='q'||key==27)//exit part the program run until pressed esc or q keys 
		{
			break;
		}
	
	}

	return 0;
}
