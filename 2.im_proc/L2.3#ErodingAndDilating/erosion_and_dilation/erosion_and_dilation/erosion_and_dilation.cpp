#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat src, erosion_dst, dilation_dst;
int erosion_elem = 0;//trackbar elements
int erosion_size = 0;
int dilation_elem = 0;
int dilation_size = 0;
int const max_elem = 2;
int const max_kernel_size = 21;

void erosion(int,void*);
void dilation(int,void*);

int main(int argc, char** argv)
{
	CommandLineParser par(argc, argv, "{@input|image.jpg|input image}");
	src = imread(samples::findFile(par.get<String>("@input")),IMREAD_COLOR );

	if (src.empty())
	{
		cout << "image cannot loading" << endl;
		cout << "usage=" << argv[0] << " image_name.jpg" << endl;
			return -1;
	}
	namedWindow("image_dilation", WINDOW_AUTOSIZE);
	namedWindow("image_erosion", WINDOW_AUTOSIZE);
	moveWindow("image_dilation", src.cols,0);

	createTrackbar("Element:\n 0:Rect \n 1:Cross \n 2:Ellipse ","image_erosion",&erosion_elem,max_elem,erosion);
	/*The function createTrackbar creates a trackbar (a slider or range control) with the specified name and range, 
	assigns a variable value to be a position synchronized with the trackbar and specifies the callback function onChange to
	be called on the trackbar position change. The created trackbar is displayed in the specified window winname.
	trackbarname-->	Name of the created trackbar.
	winname-->	Name of the window that will be used as a parent of the created trackbar.
	value-->	Optional pointer to an integer variable whose value reflects the position of the slider. Upon creation, the slider position is defined by this variable.
	count-->	Maximal position of the slider. The minimal position is always 0.
	onChange-->	Pointer to the function to be called every time the slider changes position. This function should be prototyped as void Foo(int,void*); , where the first parameter is the trackbar position and the second parameter is the user data (see the next parameter). If the callback is the NULL pointer, no callbacks are called, but only value is updated.
	userdata-->	User data that is passed as is to the callback. It can be used to handle trackbar events without using global variables.
	*/
	createTrackbar("Kernel size:\n 2n +1","image_erosion",&erosion_size,max_kernel_size,erosion);
	createTrackbar("Element:\n 0:Rect \n 1:Cross \n 2:Ellipse","image_dilation",&dilation_elem,max_elem,dilation);
	createTrackbar("Kernel size:\n 2n +1","image_dilation",&dilation_size,max_kernel_size,dilation);

	erosion(0, 0);
	dilation(0, 0);

	waitKey(0);

	return 0;
}
void erosion(int, void*)
{
	int erosion_type = 0;
	if (erosion_elem == 0) { erosion_type = MORPH_RECT; }
	else if (erosion_elem == 1) { erosion_type = MORPH_CROSS; }
	else if (erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }
	Mat element = getStructuringElement(erosion_type, Size(2 * erosion_size + 1, 2 * erosion_size + 1),Point(erosion_size,erosion_size));
	/*
	To define specified kernel we can use structring element fucntion which make a specified kernel
	 We can choose any of three shapes for our kernel:
     Rectangular box: MORPH_RECT
     Cross: MORPH_CROSS
     Ellipse: MORPH_ELLIPSE
     Then, we just have to specify the size of our kernel and the anchor point. If not specified, it is assumed to be in the center.
	 */
	erode(src, erosion_dst, element);
	/*
	src: The source image
    erosion_dst: The output image
    element: This is the kernel we will use to perform the operation. If we do not specify, the default is a simple 3x3 matrix.
    Otherwise, we can specify its shape.*/
	imshow("image_erosion", erosion_dst);
}
void dilation(int, void*)
{
	int dilation_type = 0;
	if (dilation_elem == 0) { dilation_type = MORPH_RECT; }
	else if (dilation_elem == 1) { dilation_type = MORPH_CROSS; }
	else if (dilation_elem == 2) { dilation_type = MORPH_ELLIPSE; }
    
	Mat element = getStructuringElement(dilation_type,Size(2 * dilation_size + 1, 2 * dilation_size + 1),Point(dilation_size, dilation_size));
	dilate(src, dilation_dst, element);
	imshow("image_dilation", dilation_dst);

}