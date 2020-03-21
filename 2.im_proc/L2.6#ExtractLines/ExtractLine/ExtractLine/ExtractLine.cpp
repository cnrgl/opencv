#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void show_wait_destroy(const char* win_name,Mat src);
int main(int argc,char **argv)
{
	CommandLineParser prs(argc,argv,"{@input|notes.png|image name}");
	Mat src = imread(samples::findFile(prs.get<String>("@input")), IMREAD_COLOR);
	if (src.empty())
	{
		cout << "File couldnt open!!!" << endl;
		cout << "usage=" << argv[0] << " image_name.<extension>" << endl;
		return -1;
	}
	imshow("image", src);
	Mat gray;
	if (src.channels() == 3)
	{
		cvtColor(src, gray, COLOR_BGR2GRAY);
	}
	else
	{
		gray = src;
	}
	//show image
	show_wait_destroy("gray image", gray);
	// Apply adaptiveThreshold at the bitwise_not of gray, notice the ~ symbol
	Mat bw;
	adaptiveThreshold(~gray,bw,225,ADAPTIVE_THRESH_MEAN_C,THRESH_BINARY,15,-2);
	/*
	cv.adaptiveThreshold(	src, maxValue, adaptiveMethod, thresholdType, blockSize, C[, dst]	)
	src-->	Source 8-bit single-channel image.
	dst-->	Destination image of the same size and the same type as src.
	maxValue-->	Non-zero value assigned to the pixels for which the condition is satisfied
	adaptiveMethod-->	Adaptive thresholding algorithm to use, see AdaptiveThresholdTypes. The BORDER_REPLICATE | BORDER_ISOLATED is used to process boundaries.
	thresholdType-->	Thresholding type that must be either THRESH_BINARY or THRESH_BINARY_INV, see ThresholdTypes.
	blockSize-->	Size of a pixel neighborhood that is used to calculate a threshold value for the pixel: 3, 5, 7, and so on.
	C-->	Constant subtracted from the mean or weighted mean (see the details below). Normally, it is positive but may be zero or negative as well.
	*/
	//show image
	show_wait_destroy("bw image",bw);

	// Create the images that will use to extract the horizontal and vertical lines
	Mat horizontal = bw.clone();
	Mat vertical = bw.clone();

	// Specify size on horizontal axis
	int horizontal_size = horizontal.cols / 30;
	// Create structure element for extracting horizontal lines through morphology operations
	
	Mat horizontalStructure = getStructuringElement(MORPH_RECT,Size(horizontal_size,1),Point(-1,-1));
	//Since we want to extract horizontal lines , we choose element like as horizontal line shape or directly rect which is high one.


	// Apply morphology operations/opening 
	erode(horizontal, horizontal, horizontalStructure,Point(-1,-1));
	dilate(horizontal,horizontal,horizontalStructure,Point(-1,-1));
	// Show extracted horizontal lines
	show_wait_destroy("horizontal",horizontal);
	// Specify size on vertical axis
	int vertical_size = vertical.rows / 30;
	// Create structure element for extracting vertical lines through morphology operations

	Mat verticalStructure = getStructuringElement(MORPH_RECT,Size(1,vertical_size),Point(-1,-1));
	//The same applies for the vertical lines, with the corresponding structure element

	//Apply morphology operations/
	erode(vertical,vertical,verticalStructure,Point(-1,-1));
	dilate(vertical,vertical,verticalStructure,Point(-1,-1));
	//show extracted vertical lines
	show_wait_destroy("vertical",vertical);
	//inverse vertical image/we want to image original gray scale form so we take inverse of vertical
	bitwise_not(vertical, vertical);
	show_wait_destroy("vertical bitwise",vertical);
	//we need to refine the edges in order to obtain a smoother result
	// Extract edges and smooth image according to the logic
   // 1. extract edges
   // 2. dilate(edges)
   // 3. src.copyTo(smooth)
   // 4. blur smooth img
   // 5. smooth.copyTo(src, edges)
	//Step 1
	Mat edges;
	adaptiveThreshold(vertical, edges, 225, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 3, -2);//this is a filter actually
	//beacause inner shape mean value high ,so if apply adaptiveThreshold this image inner shape will erase.so lefovers give us a contour of shape 
	show_wait_destroy("edges", edges);
	//Step 2
	Mat kernel = Mat::ones(2,2,CV_8UC1);
	dilate(edges, edges, kernel);
	show_wait_destroy("dilate", edges);
	//Step 3
	Mat smooth = vertical.clone();
	//Step 4
	blur(smooth,smooth,Size(2,2));
	//Step 5
	smooth.copyTo(vertical, edges);
	/*cv::Mat::copyTo(OutputArray m,InputArray mask)const
	m-->	Destination matrix. If it does not have a proper size or type before the operation, it is reallocated.
	mask-->	Operation mask of the same size as *this. Its non-zero elements indicate which matrix elements need to be copied.
			The mask has to be of type CV_8U and can have 1 or multiple channels.
*/
	//final - show
	show_wait_destroy("final - show",vertical);
	return 0;
}
void show_wait_destroy(const char* win_name,Mat src)
{
	imshow(win_name, src);
	moveWindow(win_name, 0, 500);
	waitKey(0);
	destroyWindow(win_name);
}