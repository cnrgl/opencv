#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
/*We will briefly explore two ways of defining the extra padding (border) for an image:

BORDER_CONSTANT: Pad the image with a constant value(i.e.black or 0
	BORDER_REPLICATE: The row or column at the very edge of the original is replicated to the extra border.
	*/
using namespace cv;


Mat src, dst;
int top, bottom, left, right;
int border_type = BORDER_CONSTANT;
const char* window_name = "border demo";
RNG rng(12345);

int main(int argc,char **argv)
{
	const char* imageName = argc >= 2 ? argv[1] : "image.jpg";
	// Loads an image
	src = imread(samples::findFile(imageName), IMREAD_COLOR); // Load an image
	// Check if image is loaded fine
	if (src.empty()) {
		printf(" Error opening image\n");
		printf(" Program Arguments: [image_name -- default lena.jpg] \n");
		return -1;
	}

	// Brief how-to for this program
	printf("\n \t copyMakeBorder Demo: \n");
	printf("\t -------------------- \n");
	printf(" ** Press 'c' to set the border to a random constant value \n");
	printf(" ** Press 'r' to set the border to be replicated \n");
	printf(" ** Press 'ESC' to exit the program \n");

	namedWindow(window_name, WINDOW_AUTOSIZE);
	//Now we initialize the argument that defines the size of the borders (top, bottom, left and right).
	top = (int)(0.05 * src.rows); bottom = top;
	left = (int)(0.05 * src.cols); right = left;

	for (;;)
	{
		//In each iteration (after 0.5 seconds), the random border color (value) is updated...
		Scalar value(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255));

		copyMakeBorder(src,dst,top,bottom,left,right,border_type,value);
		/*
		The function copies the source image into the middle of the destination image. The areas to the left, to the right,
		above and below the copied source image will be filled with extrapolated pixels. This is not what filtering functions based on it do (they extrapolate pixels on-fly),
		but what other more complex functions, including your own, may do to simplify image boundary handling.

		 copyMakeBorder( src, dst, top, bottom, left, right, borderType, value );
		 src: Source image
		 dst: Destination image
		 top, bottom, left, right: Length in pixels of the borders at each side of the image. We define them as being 5% of the original size of the image.
		 borderType: Define what type of border is applied. It can be constant or replicate for this example.
		 value: If borderType is BORDER_CONSTANT, this is the value used to fill the border pixels.
		 */
		imshow(window_name, dst);

		char key = waitKey(500);
		if(key==27)
		{
			break;
		}
		else if(key=='c')
		{
			border_type = BORDER_CONSTANT;
		}
		else if(key=='r')
		{
			border_type = BORDER_REPLICATE;
		}
	}

	return 0;
}