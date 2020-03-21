#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using namespace std;

int main(int argc,char** argv)
{
	Mat src, src_gray, dst,dst_abs;
	int ksize = 3;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;

	const char* window_name = "Laplacian operation";
	const char* filename = argc > 1 ? argv[1] : "lena.jpg";
	
	src = imread(samples::findFile(filename),IMREAD_COLOR);

	if (src.empty())
	{
		printf("file couldnt open...\n");
		printf(" Program Arguments: [image_name -- default lena.jpg] \n");
		return -1;
	}
	// Reduce noise by blurring with a Gaussian filter ( kernel size = 3 ) -- in order to reduce noise in image
	GaussianBlur(src, src, Size(3,3), 0, 0, BORDER_DEFAULT);

	cvtColor(src,src_gray,COLOR_BGR2GRAY);
	Laplacian(src_gray, dst, ddepth, ksize, scale, delta, BORDER_DEFAULT);
	// converting back to CV_8U
	convertScaleAbs(dst,dst_abs);

	imshow(window_name,dst_abs);
	waitKey(0);

	return 0;
}
