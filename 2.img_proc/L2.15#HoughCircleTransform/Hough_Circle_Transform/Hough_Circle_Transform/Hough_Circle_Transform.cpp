#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
/*
This process can be divided into two stages. The first stage is fixing radius then find the optimal center of circles in a 2D parameter space. 
The second stage is to find the optimal radius in a one dimensional parameter space.

for each points of circles ,there is a circle (whose radius is known) in grid (use for counting circle and add in accumulator matrix) drawing.
after drawing many of circle in grid ,determine the local maxima which is show center of circle in image by increase voting number using intersection between circles in grid.
Voting number is equal treshold value that indicates how many number points in circle acceptable.  
    For each A[a,b,r] = 0;

	voting=
1)Process the filtering algorithm on image Gaussian Blurring, convert the image to grayscale ( grayScaling), make Canny operator, The Canny operator gives the edges on image.
2)Vote the all possible circles in accumulator.
3)The local maximum voted circles of Accumulator A gives the circle Hough space.
4)The maximum voted circle of Accumulator gives the circle.
*/
using namespace cv;
int main(int argc,char **argv)
{
	const char* file_name = argc >= 2 ? argv[1] : "./image.jpg";
	Mat src = imread(samples::findFile(file_name),IMREAD_COLOR);
	if (src.empty()) {
		printf(" Error opening image\n");
		printf(" Program Arguments: [image_name -- default %s] \n", file_name);
		return EXIT_FAILURE;
	}
	Mat gray;
	cvtColor(src,gray,COLOR_BGR2GRAY);

	medianBlur(gray,gray,5);//Apply a Median blur to reduce noise and avoid false circle detection

	std::vector<Vec3f> circles;
	HoughCircles(gray,circles,HOUGH_GRADIENT,1,
		                     gray.rows/16,100,30,1,30);
	/*
	HoughCircles( InputArray image, OutputArray circles,
							   int method, double dp, double minDist,
							   double param1 = 100, double param2 = 100,
							   int minRadius = 0, int maxRadius = 0 );

	gray: Input image (grayscale).
	circles: A vector that stores sets of 3 values: xc,yc,r for each detected circle.
	HOUGH_GRADIENT: Define the detection method. Currently this is the only one available in OpenCV.
	dp = 1: The inverse ratio of resolution.
	min_dist = gray.rows/16: Minimum distance between detected centers.
	param_1 = 200: Upper threshold for the internal Canny edge detector.
	param_2 = 100*: Threshold for center detection.//min point for acceptable wheter this is a circle or not 
	min_radius = 0: Minimum radius to be detected. If unknown, put zero as default.
	max_radius = 0: Maximum radius to be detected. If unknown, put zero as default.
	*/
	//draw circles
	for (size_t i = 0; i < circles.size(); i++)
	{
		Vec3i c = circles[i];
		Point center = Point(c[0],c[1]);
		int radius = c[2];
		circle(src,center,1,Scalar(0,100,100),3,LINE_AA); // circle center
		circle(src,center,radius,Scalar(255,0,255),3,LINE_AA);// circle outline
	}
	imshow("detected circles", src);
	waitKey();
	return 0;
}

