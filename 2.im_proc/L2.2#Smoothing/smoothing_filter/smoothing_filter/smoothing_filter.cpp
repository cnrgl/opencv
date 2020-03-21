#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>


int DELAY_CAPTION = 1500;
int DELAY_BLUR = 100;
int MAX_KERNEL_LENGTH = 30;

using namespace std;
using namespace cv;

char window_name[] = "Smooth image";
Mat src, dst;

int display_caption(const char*);//write text on the screen
int display_dst(int del=DELAY_CAPTION);

int main(int argc,char **argv)
{
	namedWindow(window_name,WINDOW_AUTOSIZE);
	const char *image_name = (argc > 1) ? argv[1] : "image.jpg";
	src = imread(samples::findFile(image_name),IMREAD_COLOR);

	if (src.empty())
	{
		printf(" Error opening image\n");
		printf(" Usage:\n %s [image_name-- image.jpg] \n", argv[0]);
		return EXIT_FAILURE;
	}
	if(display_caption("ORIGINAL IMAGE")!=0)
	{
		return 0;	
	}
	dst = src.clone();
	if(display_dst()!=0)
	{
		return 0;
	}
	if(display_caption("Homogenus Blur")!=0)
	{
		return 0;
	}
	for (int i = 1; i < MAX_KERNEL_LENGTH; i+=2)
	{
		blur(src, dst, Size(i, i), Point(-1,-1));//blur(src,dst,ksize,anchor_point)
		if(display_dst()!=0)
		{
			return 0;
		}
	}
	if(display_caption("Gaussian Filter")!=0)
	{
		return 0;
	}
	for (int i = 1; i < MAX_KERNEL_LENGTH; i += 2)
	{
		GaussianBlur(src, dst, Size(i, i), 0, 0);//normalized gauss applied sigma=0 GaussianBlur(src,dst,ksize,sigmax,sigmay)
		//The size of the kernel to be used (the neighbors to be considered). 
		//w and h have to be odd and positive numbers otherwise the size will be calculated using the σx and σy arguments.
		if (display_dst() != 0)
		{
			return 0;
		}
	}
	if(display_caption("Median Filter")!=0)
	{
		return 0;
	}
	for (int i = 1; i < MAX_KERNEL_LENGTH; i += 2)
	{
		medianBlur(src, dst, i);//third argument is size of kernel that must be square
		if(display_dst()!=0)
		{
			return 0;
		}
	}
	if(display_caption("Bilateral Filter")!=0)
	{
		return 0;
	}
	for (int i = 1; i < MAX_KERNEL_LENGTH; i += 2)
	{
		bilateralFilter(src, dst,i,i*2,i/2);//bilateralFilter(src,dst,d,σColor,σSpace)
		/*
		  src: Source image
          dst: Destination image
          d: The diameter of each pixel neighborhood.
		  σColor: Standard deviation in the color space.

		  A larger value of the parameter means that farther colors within the pixel neighborhood (see sigmaSpace) will be mixed together, 
		  resulting in larger areas of semi-equal color.

	      σSpace: Standard deviation in the coordinate space(in pixel terms)
			Filter sigma in the coordinate space. A larger value of the parameter means that farther pixels will influence 
			each other as long as their colors are close enough (see sigmaColor ). When d>0, it specifies the neighborhood 
			size regardless of sigmaSpace. Otherwise, d is proportional to sigmaSpace.

	     */
        if(display_dst()!=0)
		{
			return 0;
		}
	}

	display_caption("displaying has done");
	return 0;
}
int display_caption(const char *capt)
{
	dst = Mat::zeros(src.size(),src.type());
	putText(dst, capt, Point(src.cols / 3, src.rows / 2), FONT_HERSHEY_COMPLEX, 1, Scalar(200,200,200), 3, LINE_8);
	return display_dst(DELAY_CAPTION);
}
int display_dst(int del)
{
	imshow(window_name, dst);
	if (waitKey(del) >= 0) { return -1; }//interrupt running
	return 0;
}