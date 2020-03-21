#include <iostream>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
/*
 ***set threshold which shows number point lie on the line.***

 ***in polar axis definition,we could drawing a plot which shows points of lines vector by choosing fixed point in axis ,in other word we could draw lines vectors by using parametries
 in polar space which pass through fixed point which we determined in advance.
 ***if we want to get a line which pass through two or more point which is known ,we draw this parametric polar plot and determine line vector by determine these axis 
 intersection point.

 line can be detected by finding the number of intersections between curves.
 The more curves intersecting means that the line represented by that intersection have more points. 
 In general, we can define a threshold of the minimum number of intersections needed to detect a line.

 ***in other word we define threshold of number which number is shows point lie on the line...

  It keeps track of the intersection between curves of every point in the image.
 if number of intersection is above some threshold ,then it declares it is line with the parameters (teta,ro)of intersection point.
****The Standard Hough Transform*******

	It consists in pretty much what we just explained in the previous section. It gives you as result a vector of couples (θ,rθ)
	In OpenCV it is implemented with the function HoughLines()


****The Probabilistic Hough Line Transform*****

	A more efficient implementation of the Hough Line Transform. It gives as output the extremes of the detected lines (x0,y0,x1,y1)
	In OpenCV it is implemented with the function HoughLinesP()
	
	*/
using namespace cv;
using namespace std;

int main(int argc,char **argv)
{
	Mat dst, cdst, cdstP;

	const char* default_file = "./image.jpg";
	const char* filename = argc >= 2 ? argv[1] : default_file;
	// Loads an image
	Mat src = imread(samples::findFile(filename), IMREAD_GRAYSCALE);
	// Check if image is loaded fine
	if (src.empty()) {
		printf(" Error opening image\n");
		printf(" Program Arguments: [image_name -- default %s] \n", default_file);
		return -1;
	}

	Canny(src, dst, 50, 200,3 );//canny applied

	cvtColor(dst,cdst,COLOR_GRAY2BGR);
	cdstP = cdst.clone();

	vector<Vec2f>lines;
	HoughLines(dst, lines, 1, CV_PI / 180,150,0,0);//dst must be the type of BGR (CV_8UC1)
	/*
	This function use such numerical methods when it determine intesection between lines by move forward as much as size of the resolution.   
	HoughLines	(	InputArray 	image,             /	8-bit, single-channel binary source image. The image may be modified by the function.
					OutputArray 	lines,         /    Output vector of lines. Each line is represented by a 2 or 3 element vector (ρ,θ) or (ρ,θ,votes) . ρ is the distance from the coordinate origin (0,0) (top-left corner of the image). θ is the line rotation angle in radians ( 0∼vertical line,π/2∼horizontal line ). votes is the value of accumulator.
					double 	rho,                   /    The resolution of the parameter r in pixels. We use 1 pixel.
					double 	theta,                 /    Angle resolution of the accumulator in radians.
					int 	threshold,			   /    Accumulator threshold parameter. Only those lines are returned that get enough votes ( >threshold ).
					double 	srn = 0,			   /    For the multi-scale Hough transform, it is a divisor for the distance resolution rho . The coarse accumulator distance resolution is rho and the accurate accumulator resolution is rho/srn . If both srn=0 and stn=0 , the classical Hough transform is used. Otherwise, both these parameters should be positive.
					double 	stn = 0,			   /    For the multi-scale Hough transform, it is a divisor for the distance resolution theta.
					double 	min_theta = 0,		   /    For standard and multi-scale Hough transform, minimum angle to check for lines. Must fall between 0 and max_theta.
					double 	max_theta = CV_PI	   /	For standard and multi-scale Hough transform, maximum angle to check for lines. Must fall between min_theta and CV_PI.
				)
*/
	for (size_t i=0;i<lines.size();i++)
	{
		float ro = lines[i][0], teta = lines[i][1];//ro and teta is belong to lines output vectors
		Point p1, p2;
		double a = cos(teta),b=sin(teta);
		double x0 = a * ro, y0 = b * ro;
		p1.x = cvRound(x0+1000*(-b));//x0 and y0 show center of line so we must strecth the line with factor 1000
		p1.y = cvRound(y0+1000*(a));
		p2.x = cvRound(x0-1000*(-b));
		p2.y = cvRound(y0-1000*a);
		line(cdst,p1,p2,Scalar(0,0,255),1,LINE_AA);
	}
	vector<Vec4i> linePi;
	HoughLinesP(dst,linePi,1,CV_PI/180,50,50,10);
	for(size_t i=0;i<linePi.size();i++)//show lines
	{ 
		Vec4i l = linePi[i];
		line(cdstP,Point(l[0],l[1]),Point(l[2],l[3]),Scalar(0,0,225),3,LINE_AA);
	}

	imshow("source",src);
	imshow("Detected Lines(in red) - Standart Hough Line Transform",cdst);
	imshow("Detected Lines(in red) - Probabilistic Line Transform ",cdstP);
	waitKey(0);
	return 0;
}