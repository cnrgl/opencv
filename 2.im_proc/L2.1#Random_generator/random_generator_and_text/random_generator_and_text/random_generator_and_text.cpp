#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

const int window_height=500;
const int  window_width=600;

const int Delay = 5;
const int NUMBER = 100;
int x_1 = -window_width / 2;//uniform random space size x_2-x_1
int x_2 = window_width * 3 / 2;
int y_1 = -window_width / 2;
int y_2 = window_width * 3 / 2;

using namespace cv;

static Scalar randomColor(RNG &rn);//use internal rng member 
int Drawing_Random_Lines(Mat image, char* window_name, RNG rng);
int Drawing_Random_Rectangles(Mat image, char* window_name, RNG rng);
int Drawing_Random_Ellipses(Mat image, char* window_name, RNG rng);
int Drawing_Random_Polylines(Mat image, char* window_name, RNG rng);
int Drawing_Random_Filled_Polygons(Mat image, char* window_name, RNG rng);
int Drawing_Random_Circles(Mat image, char* window_name, RNG rng);
int Displaying_Random_Text(Mat image, char* window_name, RNG rng);
int Displaying_Big_End(Mat image, char* window_name, RNG rng);

int main(void)
{
	int c;
	char window[] = "random_image";
	RNG rng(0xFFFFFFFF);//64-bit random number generator initialized with 0xFFFFFFFF
	Mat img = Mat::zeros(window_height,window_width,CV_8UC3);//all image is black for now...
	imshow(window,img);
	waitKey(Delay);

	 c = Drawing_Random_Lines(img,window,rng);
	 if (c != 0) { waitKey(0); return 0; }
	 c = Drawing_Random_Ellipses(img, window, rng);
	if (c != 0) { waitKey(0); return 0; }//if function hasnt closed yet
	c = Drawing_Random_Rectangles(img, window, rng);
	if (c != 0) { waitKey(0); return 0; }
	c = Drawing_Random_Polylines(img, window, rng);
	if (c != 0) { waitKey(0); return 0; }
	c = Drawing_Random_Filled_Polygons(img, window, rng);
	if (c != 0) { waitKey(0); return 0; }
	c = Drawing_Random_Circles(img, window, rng);
	if (c != 0) { waitKey(0); return 0; }
	c = Displaying_Random_Text(img, window, rng);
	if (c != 0) { waitKey(0); return 0; }
	c = Displaying_Big_End(img, window, rng);
	if (c != 0) { waitKey(0); return 0; }

	waitKey(0);
	return 0;
}
static Scalar randomColor(RNG &rng)
{
	//the return value is an Scalar with 3 randomly initialized values, which are used as the R, G and B parameters for the line color. 
	//Hence, the color of the lines will be random too!
	int icolor = (unsigned)rng;//not uniformly distributed but randomlys generated number
	return Scalar(icolor&225,(icolor>>4)&225,(icolor>>8)&225);//BGR values with randomly generated and shifted from other ones with different values
}
int Drawing_Random_Lines(Mat image, char* window_name, RNG rn)
{
	Point pt1, pt2;
	for (int i = 0; i < NUMBER; i++)//The for loop will repeat NUMBER times.
	{
		pt1.x = rn.uniform(x_1,x_2);//rn uniform member function generates number randomly uniform distribution between the values x_1 and x_2
		pt1.y = rn.uniform(y_1, y_2);
		pt2.x = rn.uniform(x_1, x_2);
		pt2.y = rn.uniform(y_1, y_2);
		line(image, pt1, pt2, randomColor(rn),rn.uniform(1,10),LINE_8);
		imshow(window_name,image);
		if (waitKey(Delay) >= 0) { return -1; }//if key pressed waitKey return positive value
	}
	return 0;//each window has one unique distribution
}
//he parameters such as center and vertices are also generated randomly in the next examples...
int Drawing_Random_Rectangles(Mat image,char *window,RNG rng)
{
	Point pt1, pt2;
	int Line_type = 8;
	int thickness = rng.uniform(-3, 10);//thickness must be and above -1  
	for (int i = 0; i < NUMBER; i++) {
		pt1.x = rng.uniform(x_1, x_2);
		pt1.y = rng.uniform(y_1, y_2);
		pt2.x = rng.uniform(x_1, x_2);
		pt2.y = rng.uniform(y_1, y_2);
		rectangle(image, pt1, pt2, randomColor(rng), MAX(thickness, -1), Line_type);
		imshow(window, image);
		if (waitKey(Delay) >= 0) { return -1; }
	}
	return 0;
}
int Drawing_Random_Ellipses(Mat image, char* window_name, RNG rng)
{
	int lineType = 8;
	for (int i = 0; i < NUMBER; i++)
	{
		Point center;
		center.x = rng.uniform(x_1, x_2);
		center.y = rng.uniform(y_1, y_2);
		Size axes;
		axes.width = rng.uniform(0, 200);
		axes.height = rng.uniform(0, 200);
		double angle = rng.uniform(0, 180);
		ellipse(image, center, axes, angle, angle - 100, angle + 200,
			randomColor(rng), rng.uniform(-1, 9), lineType);
		imshow(window_name, image);
		if (waitKey(Delay) >= 0)
		{
			return -1;
		}
	}
	return 0;
}
int Drawing_Random_Polylines(Mat image, char* window_name, RNG rng)
{
	int lineType = 8;
	for (int i = 0; i < NUMBER; i++)
	{
		Point pt[2][3];//triangle
		pt[0][0].x = rng.uniform(x_1, x_2);
		pt[0][1].x = rng.uniform(x_1, x_2);
		pt[0][2].x = rng.uniform(x_1, x_2);
		pt[1][0].x = rng.uniform(x_1, x_2);
		pt[1][1].x = rng.uniform(x_1, x_2);
		pt[1][2].x = rng.uniform(x_1, x_2);
		pt[0][0].y = rng.uniform(y_1, y_2);
		pt[0][1].y = rng.uniform(y_1, y_2);
		pt[0][2].y = rng.uniform(y_1, y_2);
		pt[1][0].y = rng.uniform(y_1, y_2);
		pt[1][1].y = rng.uniform(y_1, y_2);
		pt[1][2].y = rng.uniform(y_1, y_2);

		const Point* ppt[2] = {pt[0],pt[1]};
		int npt[] = { 3,3 };//number of point
		polylines(image, ppt, npt, 2, true, randomColor(rng), rng.uniform(1, 10), lineType);
		//polylines	(InputOutputArray img,const Point* const* pts,const int* npts,int ncontours,bool isClosed,const Scalar& color,int thickness = 1,int lineType = LINE_8,int shift = 0)

		imshow(window_name, image);
		if (waitKey(Delay) >= 0) { return -1; }
	}
	return 0;
}
int Drawing_Random_Filled_Polygons(Mat image, char* window_name, RNG rng)
{
	int lineType = 8;
	for (int i = 0; i < NUMBER; i++)
	{
		Point pt[2][3];
		pt[0][0].x = rng.uniform(x_1, x_2);
		pt[0][0].y = rng.uniform(y_1, y_2);
		pt[0][1].x = rng.uniform(x_1, x_2);
		pt[0][1].y = rng.uniform(y_1, y_2);
		pt[0][2].x = rng.uniform(x_1, x_2);
		pt[0][2].y = rng.uniform(y_1, y_2);
		pt[1][0].x = rng.uniform(x_1, x_2);
		pt[1][0].y = rng.uniform(y_1, y_2);
		pt[1][1].x = rng.uniform(x_1, x_2);
		pt[1][1].y = rng.uniform(y_1, y_2);
		pt[1][2].x = rng.uniform(x_1, x_2);
		pt[1][2].y = rng.uniform(y_1, y_2);
		const Point* ppt[2] = { pt[0], pt[1] };
		int npt[] = { 3, 3 };
		fillPoly(image, ppt, npt, 2, randomColor(rng), lineType);
		imshow(window_name, image);
		if (waitKey(Delay) >= 0)
		{
			return -1;
		}
	}
	return 0;
}
int Drawing_Random_Circles(Mat image, char* window_name, RNG rng)
{
	int lineType = 8;
	for (int i = 0; i < NUMBER; i++)
	{
		Point center;
		center.x = rng.uniform(x_1, x_2);
		center.y = rng.uniform(y_1, y_2);
		circle(image, center, rng.uniform(0, 300), randomColor(rng),
			rng.uniform(-1, 9), lineType);
		imshow(window_name, image);
		if (waitKey(Delay) >= 0)
		{
			return -1;
		}
	}
	return 0;
}
int Displaying_Random_Text(Mat image, char* window_name, RNG rng)
{
	int lineType = 8;
	for (int i=0;i<NUMBER;i++)
	{
		Point org;
		org.x = rng.uniform(x_1,x_2);
		org.y = rng.uniform(y_1, y_2);

		putText(image,
			"RandomText",//text which is writing *char[rng.uniform(0,n)];also supported random text
			org,//The bottom-left corner of the text will be located in the Point org
			rng.uniform(0, 8),//Font type random choosed in range 0-8
			rng.uniform(0, 100) * 0.05 + 0.01,//font size which meaning range is: [0.1,5.1>
			randomColor(rng),//random generated color
			rng.uniform(1,10),//thickness
			lineType
		);
		if (waitKey(Delay) >= 0) { return - 1; }
	}
	return 0;
}
int Displaying_Big_End(Mat image, char* window_name, RNG rng)
{
	Size text_size = getTextSize("OpenCV!",FONT_HERSHEY_COMPLEX,3,5,0);//text	Input text string.
	    /*cv::getTextSize(	text, fontFace, fontScale, thickness,BaseLine)
		fontFace-->	Font to use, see HersheyFonts.
		fontScale -->Font scale factor that is multiplied by the font - specific base size.
		thickness -->	Thickness of lines used to render the text.See putText for details.
		baseLine  -->	y - coordinate of the baseline relative to the bottom - most text point.
		*/
	Point org((window_width - text_size.width) / 2, (window_height - text_size.height) / 2);
	int lineType = 8;

	Mat image2;//in the same time we change image color font with varying i values
	for (int i = 0; i < NUMBER; i+=2)
	{
		image2 = image - Scalar::all(i);//varying with i
		putText(image2,"OpenCV!",org,FONT_HERSHEY_COMPLEX,3,Scalar(i,i,MIN(i+150,225)),5,lineType);
		imshow(window_name,image2);
		if (waitKey(Delay) >= 0) { return -1; }
	}
	/*
	So, image2 is the subtraction of image and Scalar::all(i).
	In fact, what happens here is that every pixel of image2 will be the result of subtracting every pixel of image minus the value of i 
	(remember that for each pixel we are considering three values such as R, G and B, so each of them will be affected)

	Also remember that the subtraction operation always performs internally a saturate operation, which means that the result 
	obtained will always be inside the allowed range(no negative and between 0 and 255 for our example).
	*/
	return 0;
}