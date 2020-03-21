#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#define w 400

using namespace cv;

void MyEllipse(Mat img,double angle);
void MyFilledCircle(Mat img,Point center);
void MyPolygon(Mat img);
void MyLine(Mat img,Point start,Point end);

int main(void)
{
	/*
	point pt; 
	pt.x;pt.y;
	or
	point pt=cv::Point(x,y);
	*/
	/*
	cv::Scalar(a,b,c);
	*/
	//Since we plan to draw two examples (an atom and a rook), we have to create two images and two windows to display them.
	char image1[] = "atom";
	char image2[] = "rook";

	Mat atom = Mat::zeros(w,w,CV_8UC3);
	Mat rook = Mat::zeros(w,w,CV_8UC3);

	MyEllipse(atom,90);
	MyEllipse(atom,0);
	MyEllipse(atom,-45);
	MyEllipse(atom,45);

	MyFilledCircle(atom,Point(w/2,w/2));
	//drawing the rook image
	MyPolygon(rook);

	rectangle(rook,
		     Point(0,7*w/8),//vertices one
		     Point(w,w),//opposite vertices
		     Scalar(0,225,225),//yellow
		     FILLED,//thickness=-1
		     LINE_8);

	imshow(image1,atom);
	moveWindow(image1,50,200);//move window specified position
	imshow(image2,rook);
	moveWindow(image2,50+w,200);

	waitKey();

	return 0;
}
void MyLine(Mat img,Point start,Point end)
{
	int thickness = 2;
	int lineType = LINE_8;
	line(img,
		start,
		end,
		Scalar(0,0,0),//BGR black line
		thickness,
		lineType);
}
void MyEllipse(Mat img,double angle)
{
	int thickness = 2;
	int lineType = LINE_8;
	ellipse(img,
		    Point(w/2,w/2),
		    Size(w/4,w/6),//The ellipse center is located in the point (w/2, w/2) and is enclosed in a box of size (w/4, w/16)
		    angle,//The ellipse is rotated angle degrees
		    0,//start angle
		    360,//end angle,The ellipse extends an arc between 0 and 360 degrees
		    Scalar(225,0,0),//BGR values BLUE
		    thickness,//=2
		    lineType
		);
}
void MyFilledCircle(Mat img,Point center)
{
	circle(img,
		   center,//center of circle
           w/32,//radius
		   Scalar(0,0,225),//BGR RED
		   FILLED,//Thickness-->FILLED=-1Since thickness = -1, the circle will be drawn filled.
		   LINE_8
	);
}
void MyPolygon(Mat img)
{
	int lineType = LINE_8;
	Point rook_points[1][20];
	rook_points[0][0] = Point(w / 4, 7 * w / 8);
	rook_points[0][1] = Point(3 * w / 4, 7 * w / 8);
	rook_points[0][2] = Point(3 * w / 4, 13 * w / 16);
	rook_points[0][3] = Point(11 * w / 16, 13 * w / 16);
	rook_points[0][4] = Point(19 * w / 32, 3 * w / 8);
	rook_points[0][5] = Point(3 * w / 4, 3 * w / 8);
	rook_points[0][6] = Point(3 * w / 4, w / 8);
	rook_points[0][7] = Point(26 * w / 40, w / 8);
	rook_points[0][8] = Point(26 * w / 40, w / 4);
	rook_points[0][9] = Point(22 * w / 40, w / 4);
	rook_points[0][10] = Point(22 * w / 40, w / 8);
	rook_points[0][11] = Point(18 * w / 40, w / 8);
	rook_points[0][12] = Point(18 * w / 40, w / 4);
	rook_points[0][13] = Point(14 * w / 40, w / 4);
	rook_points[0][14] = Point(14 * w / 40, w / 8);
	rook_points[0][15] = Point(w / 4, w / 8);
	rook_points[0][16] = Point(w / 4, 3 * w / 8);
	rook_points[0][17] = Point(13 * w / 32, 3 * w / 8);
	rook_points[0][18] = Point(5 * w / 16, 13 * w / 16);
	rook_points[0][19] = Point(w / 4, 13 * w / 16);

	const Point* ppt[1] = { rook_points[0] };
	int npt[] = { 20 };

	fillPoly(img,
		     ppt,//vertices of polygon
		     npt,//number of points
		     1,
		     Scalar(225,225,225),//color white
		     lineType
		);

}