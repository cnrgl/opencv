#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	Mat im;
	//INPUT/OUTPUT----------------------------------------------------------------------------------
	im = imread("./image.jpg");//imread scan image 3 channel default
	//imread("./image.jpg",IMREAD_GRAYSCALE)-->one channel if needed
	imwrite("./image2.jpg",im);//save image
	//Use cv::imdecode and cv::imencode to read and write an image from/to memory(buffer) rather than a file.

	//ACCESING PIXEL VALUES------------------------------------------------------------------------
	//In order to get pixel intensity value, you have to know the type of an image and the number of channels.
	//GRAY SCALE
	Scalar intensity = im.at<uchar>(0,0);//at retrieves data from matrix 
	//im.at<uchar>(Point(x,y))
	//3channel(BGR)
	Vec3b intensity3 = im.at<Vec3b>(0, 0);
	uchar blue = intensity3[0];
	uchar green = intensity3[1];
	uchar red = intensity3[2];
	//im.at<Point2f>(i,j);
	Vec3b* in = im.ptr<Vec3b>();//vector pointer assigning
	//There are functions in OpenCV, especially from calib3d module, such as cv::projectPoints, that take an array of 2D or 3D points in the form of Mat
		/*we can fill matrix by hand
		    vector<Point2f> points;
			//... fill the array
			Mat pointsMat = Mat(points);
		*/
	//FALSE
	vector<Point3f>points;
	Mat im2 = Mat(points);//this will result one row(Gray scale image) but we use 3 channel image
	//TRUE
	im2 = im.clone();

	//his method allocates data for a matrix if it is empty. If it is not empty and has the correct size and type, the method does nothing. 
	//If however, size or type are different from the input arguments, the data is deallocated (and lost) and a new data is allocated. 
	Mat sobelx;
	Sobel(im, sobelx, CV_32F, 1, 0);//sobel filter
	
    //Primitive operations--------------------------------------------------------------------------
	//There is a number of convenient operators defined on a matrix. 
	im = Scalar::all(0);
	//Selecting a region of interest:
	Rect r(10,10,100,100);
	Mat ROI = im(r);
	//Conversion from color to greyscale:
	Mat gray_im;
	cvtColor(im,gray_im,COLOR_BGR2GRAY);
	//Change image type from 8UC1 to 32FC1:
	Mat dst;
	im.convertTo(dst, CV_8U);

	//VISUALIZING------------------------------------------------------------------------------------
	//It is very useful to see intermediate results of your algorithm during development process. OpenCV provides a convenient way of visualizing images.
	Mat img = imread("image.jpg");
	namedWindow("image",WINDOW_AUTOSIZE);
	imshow("image",img);
	waitKey(0);
	cvtColor(img,gray_im,COLOR_BGR2GRAY);
	Sobel(gray_im,sobelx,CV_32F,1,0);
	double min, max;
	minMaxLoc(sobelx,&min,&max);//find minimum and maximum intensities
    Mat draw;
	sobelx.convertTo(draw, CV_8U, 255.0 / (max - min), -min * 255.0 / (max - min));//alpha=max saturation value ,beta=min saturation value 
	namedWindow("image_filtered",WINDOW_AUTOSIZE);
	imshow("image_filtered",draw);
	waitKey(0);


	return 0;
}