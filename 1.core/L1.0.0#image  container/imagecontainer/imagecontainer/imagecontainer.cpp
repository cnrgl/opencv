#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;
int main(int argc,char**argv)
{
	Mat A, B;
	A = imread(argv[1],IMREAD_COLOR);//image allocation

	Mat C(A);/*copy constructor use for copy just A object header in C object structure data.
	          So as an any change in C will affect both A matrix data.
		     */
	B = A;//assigment operator do same think like as copy constructor  

	Mat D(A,Rect(10,10,100,100));//ROI(range of interest) - copy to subsection area of data that we interest
	Mat E = A(Range::all(),Range(1,3));
	
	//whenever any copy is erased, copy referance counter decreased and in the opposite situation increase counter 
	
	Mat F = A.clone();//clone function is also copy the matrix(store pixel values) itself........
	Mat G;
	A.copyTo(G);
	
/*  images store in memory in vary ways.it can be store simply grayscale method  where the colors at our disposal are black and white.
	    For colorful ways uses different methods :
		1)RGB or another name BGR (name which we are using in opencv) have three base color:blue,green,red (additional fourt element Alpha(A) for transparency)
       	2)The HSV and HLS decompose colors into their hue, saturation and value/luminance components(ton,doygunluk,parlaklık) 
		3)YCrCb is used by the popular JPEG image format.
		4)CIE L*a*b* is a perceptually uniform color space, which comes in handy if you need to measure the distance of a given color to another color.
		Also methods can be store in memory by using different type like as char(8 bit),u_int,float(4 byte),double(8 byte)...  
	*/
	
	//Mat object can be create matris itself by hand in many ways:
	/*Mat constructor Mat(rows,col,type,scalar):
	We need to specify data type use of storing the elements and number of channel per pixel: 
	--->> CV_[The number of bits per item][Signed or Unsigned][Type Prefix]C[The channel number]
	Scalar is a short vector,it can be initialize all matrix point with default values(in this example default value is 0 0 225
    Also we can define upper
	*/
	Mat M(2,2,CV_8UC3,Scalar(0,0,255));
	cout << "M = " << endl << " " << M << endl << endl;//shows the matrix 

	// Also we can use type with the upper macro  CV_8UC(1)
	//We can define matrix more than two dimension ,first define pointer with specified dimension,then pass a pointer containing the size for each dimensions
	int sz[3] = { 2,2,2 };//3 unit coloumn define for the matrix so fouth dimension has initialized
	Mat L(3, sz, CV_8UC(1), Scalar::all(0));//all channell set by zero(one channel is used)--grayscale
	
	//cv::Mat::create(row,cols,type) function:
	//with this method cannot initialize matrix values with this constructor.it will allocate if the new size will not fit into the old one
	M.create(2, 2, CV_8UC(2));
	cout << "M=" << endl << ""<< M << endl << endl;

	//MATLAB style initializer: cv::Mat::zeros , cv::Mat::ones , cv::Mat::eye . Specify size and data type to use:
	Mat Ey = Mat::eye(4, 4, CV_64F) * 0.1;//all diagonal settled 0.1
	cout << "Ey= " << endl << " " << Ey << endl << endl;
	Mat O = Mat::ones(2, 2, CV_32F);
	cout << "O= " << endl << " " << O << endl << endl;
	Mat Z = Mat::zeros(3, 3, CV_8UC1);
	cout << "Z= " << endl << " " << Z << endl << endl;

	//For small matrices you may use comma separated initializers or initializer lists (C++11 support is required)
	Mat S = (Mat_<double>(3, 3) << 0, 1, -2, 0, 1, -2, -3, 5, 9);//Mat_ class constructor invoke and initiaze values by using << operator
	cout << " S=" << endl << " " << S << endl << endl;
	S = (Mat_<double>({ 0,1,-2,0,1,-2,-3,5,9 })).reshape(3);//initiliaze with direct initializing and reshape matrix 3x3
	cout << " S=" << endl << " " << S << endl << endl;

	//copy ROI to another header or in an other saying define new header for an existing object.
	Mat kopya = S.row(1).clone();
	cout << " S=" << endl << " " << S << endl << endl;
	//You can fill out a matrix with random values using the cv::randu() function. You need to give a lower and upper limit for the random values:
	Mat R = Mat(4, 3, CV_8UC3);
	randu(R, Scalar::all(0), Scalar::all(225));



	return 0;
}