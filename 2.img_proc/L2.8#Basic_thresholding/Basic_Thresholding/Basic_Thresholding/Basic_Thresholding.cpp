#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int threshold_value=0;
int threshold_type=3;
const int max_value = 255;
const int max_type = 4;
const int max_binary = 255;

Mat src, src_gray, dst;
const char* window_name = "Threshold demo";

const char* trackbar_type = "Type: \n 0: Binary \n 1: Binary Inverted \n 2: Truncate \n 3: To Zero \n 4: To Zero Inverted";
const char* trackbar_value = "Value=";
static void Threshold_apply(int, void *) 
{ 
	/* 0: Binary
	1: Binary Inverted
	2: Threshold Truncated
	3: Threshold to Zero
	4: Threshold to Zero Inverted
   */
	/*
	src_gray: Our input image
	dst: Destination (output) image
	threshold_value: The thresh value with respect to which the thresholding operation is made
	max_BINARY_value: The value used with the Binary thresholding operations (to set the chosen pixels)
	threshold_type: One of the 5 thresholding operations. They are listed in the comment section of the function above
	*/
	threshold(src_gray, dst, threshold_value, max_binary,threshold_type);
	imshow(window_name,dst);
}
int main(int argc,char **argv)
{
	String file ("./image.png");
	
	if (argc > 1)
	{
		file = argv[1];
	}
	Mat src = imread(samples::findFile(file), IMREAD_COLOR);
	if (src.empty())
	{
		std::cout << "error while file opening..." << std::endl;
		std::cout << "usage=" << argv[0] << " file_name.jpg" << std::endl;
		return EXIT_FAILURE;
	}
	cvtColor(src, src_gray, COLOR_BGR2GRAY);
	namedWindow(window_name,WINDOW_AUTOSIZE);

	createTrackbar(trackbar_type, window_name, &threshold_type, max_type, Threshold_apply);// Create a Trackbar to choose type of Threshold

	createTrackbar(trackbar_value,window_name,&threshold_value,max_value,Threshold_apply);//create a Trackbar to adjust value of thresholding

	Threshold_apply(0,0);
	waitKey(0);

	return 0;
}