#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
/*
point follow method
start top left to searching
find other other point if there is in mask ;but isnt one sign mask for point 

*/
using namespace std;
using namespace cv;
static void help(char* progName)
{
	cout << endl
		<< "Usage:" << endl
		<< progName << " [image_path -- default image.jpg] [G -- grayscale] " << endl << endl;
}
void sharpen(const Mat& I, Mat& result);
int main(int argc, char** argv)
{
	/*Mask operations on matrices are quite simple.
	The idea is that we recalculate each pixel's value in an image according to a mask matrix (also known as kernel).
	This mask holds values that will adjust how much influence neighboring pixels (and the current pixel) have on the new pixel value.
	From a mathematical point of view we make a weighted average, with our specified values.
	*/

	help(argv[0]);
	const char* im = argc <= 2 ? im = "./image.jpg" : argv[1];
	Mat img, kernel, dest0, dest1;
	if ((argc > 2) && (!strcmp(argv[2], "G")))
		img = imread(samples::findFile(im), IMREAD_GRAYSCALE);
	else
		img = imread(samples::findFile(im), IMREAD_COLOR);

	if (img.empty())
	{
		cerr << "ERROR while" << im << "file opening" << endl;
		return EXIT_FAILURE;
	}

	namedWindow("original", WINDOW_AUTOSIZE);
	namedWindow("2dfilter", WINDOW_AUTOSIZE);

	imshow("original", img);

	double t = (double)getTickCount();
	
	sharpen(img, dest0);
	cout << "operation time is with handwriting filter=" << 1000*((double)getTickCount()-t) / getTickFrequency()<<"ms" << endl;

	imshow("2dfilter",dest0);
	waitKey(0);
	kernel = (Mat_<char>(3, 3) << 0, -1, 0
		, -1, 5, -1
		, 0, -1, 0);
	t = (double)getTickCount();
	filter2D(img, dest1, img.depth(), kernel);

	cout << "operation time is with 2dfilter=" << 1000 * ((double)getTickCount() - t) / getTickFrequency() << "ms" << endl;
	imshow("2dfilter",dest1);

	imwrite("./dest0.jpg", dest0);
	imwrite("./dest1.jpg", dest1);
	waitKey(0);

	return EXIT_SUCCESS;
}
void sharpen(const Mat& I, Mat& result)
{
	CV_Assert(I.depth() == CV_8U);
	
		
	const int nChannel = I.channels();
	result.create(I.size(), I.type());
	
	for (int i = 1; i < I.rows - 1; ++i)
	{
		const uchar* up = I.ptr<uchar>(i+1);
		const uchar* mid = I.ptr<uchar>(i);
		const uchar* down = I.ptr<uchar>(i-1);

		uchar* output = result.ptr<uchar>(i);
		for (int j = nChannel; j < nChannel * (I.cols - 1); ++j)
			*output++ = saturate_cast<uchar>((5 * mid[j]) - (up[j] + down[j] + mid[j + nChannel] + mid[j - nChannel]));//saturate_cast prevent reaches limits
	}
	result.row(0).setTo(Scalar(0));
	result.row(result.rows - 1).setTo(Scalar(0));
	result.col(0).setTo(Scalar(0));
	result.col(result.cols - 1).setTo(Scalar(0));
	
}