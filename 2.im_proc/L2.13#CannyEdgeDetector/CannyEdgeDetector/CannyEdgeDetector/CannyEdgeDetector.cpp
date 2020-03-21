#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
/*
Non-maximum suppression is applied. This removes pixels that are not considered to be part of an edge. Hence, only thin lines (candidate edges) will remain.

Hysteresis: The final step. Canny does use two thresholds (upper and lower):

	If a pixel gradient is higher than the upper threshold, the pixel is accepted as an edge
	If a pixel gradient value is below the lower threshold, then it is rejected.
	If the pixel gradient is between the two thresholds, then it will be accepted only if it is connected to a pixel that is above the upper threshold.
Canny recommended a upper:lower ratio between 2:1 and 3:1.
*/
using namespace cv;
Mat src, src_gray;
Mat dst, detected_edges;
int low_threshold;
const int max_lowThreshold = 100;
const int ratio = 3;
const int ksize = 3;
const char* window_name = "CannyDetectRatio";

void cannyThreshold(int,void *)
{
	blur(src_gray,detected_edges,Size(ksize,ksize));//noise reduction

	Canny(detected_edges,detected_edges,low_threshold,ratio*low_threshold,ksize);
	/*
	Canny	(	InputArray 	image,              /	8-bit input image.
				OutputArray 	edges,          /   output edge map; single channels 8-bit image, which has the same size as image .
				double 	threshold1,             /	first threshold for the hysteresis procedure.
				double 	threshold2,             /   second threshold for the hysteresis procedure.
				int 	apertureSize = 3,       /   aperture size for the Sobel operator.
				bool 	L2gradient = false      /   a flag, indicating whether a more accurate L2 norm =(dI/dx)2+(dI/dy)2√ 
				                                    should be used to calculate the image gradient magnitude ( L2gradient=true ), 
													or whether the default L1 norm =|dI/dx|+|dI/dy| is enough ( L2gradient=false ).
			)		
*/

	dst = Scalar::all(0);
	src.copyTo(dst,detected_edges);//the pixels is zero except edges
	imshow(window_name, dst);
}
int main(int argc, char** argv)
{
	CommandLineParser pars(argc,argv,"{@input|image.jpg|image name}");
	src = imread(samples::findFile(pars.get<String>("@input")), IMREAD_COLOR);
	if (src.empty())
	{
		std::cout << "error accured when file reading..." << std::endl;
		std::cout << "usage :" << argv[0] << " <input image>" << std::endl;
		return EXIT_FAILURE;
	}
	dst.create(src.size(), src.type());
	cvtColor(src,src_gray,COLOR_BGR2GRAY);
	namedWindow(window_name,WINDOW_AUTOSIZE);

	createTrackbar("Min Threshold:",window_name,&low_threshold,max_lowThreshold,cannyThreshold);
	cannyThreshold(0,0);

	waitKey(0);

	return 0;
}