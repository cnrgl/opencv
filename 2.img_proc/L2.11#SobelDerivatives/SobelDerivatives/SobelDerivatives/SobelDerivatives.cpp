#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>

/*
You can easily notice that in an edge, the pixel intensity changes in a notorious way.
A good way to express changes is by using derivatives. A high change in gradient indicates a major change in the image.
*/
using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	CommandLineParser pars(argc,argv,"{@input|lena.jpg|image name}"
	                                 "{ksize  k|1|ksize  (hit 'K' to increase its value at run time)}"//second of first column arguments is input option parameter (-k usage necessary)
									 "{kscale s|1|kscale (hit 'S' to increase its value at run time)}"
									 "{delta  d|0|delta  (hit 'D' to increase its value at run time)}"
									 "{help   h|false|show help message}");

	cout << "The sample uses Sobel or Scharr OpenCV functions for edge detection\n\n";
	pars.printMessage();//This method will print standard help message containing the about message and arguments description.
	cout << "\nPress 'ESC' to exit program.\nPress 'R' to reset values ( ksize will be -1 equal to Scharr function )";
	
	Mat src, src_gray, image;
	Mat grad;
	const String window_name = "Sobel operaton-derivatives";
	int ksize = pars.get<int>("ksize");
	int kscale = pars.get<int>("kscale");
	int delta = pars.get<int>("delta");
	int ddepth = CV_16S;
	
	String image_name = pars.get<String>("@input");
	image = imread(samples::findFile(image_name),IMREAD_COLOR);

	if(image.empty())
	{
		cout << "couldnt load."<<image_name<< endl;
		return -1;
	}
	for (;;)//key listener loop
	{
		// // Remove noise by blurring with a Gaussian filter ( kernel size = 3 ,sigma = 0)
		GaussianBlur(image,src,Size(3,3),0,0,BORDER_DEFAULT);
		//convert image channel type to Grayscale
		cvtColor(src, src_gray, COLOR_BGR2GRAY);
		
		Mat grad_x, grad_y;
		Mat abs_grad_x, abs_grad_y;
		//Sobel Operator
		Sobel(src_gray, grad_x, ddepth, 1, 0, ksize, kscale, delta, BORDER_DEFAULT);
		Sobel(src_gray, grad_y, ddepth, 0, 1, ksize, kscale, delta, BORDER_DEFAULT);
		/*
		In all cases except one, the ksize×ksize separable kernel is used to calculate the derivative. 
		When ksize = 1, the 3×1 or 1×3 kernel is used (that is, no Gaussian smoothing is done). ksize = 1 can only be used for the first or the second x- or y- derivatives.

		scharr filter::::
		There is also the special value ksize = FILTER_SCHARR (-1) that corresponds to the 3×3 Scharr filter that may give more accurate results than the 3×3 Sobel.

		 cv::Sobel	(	InputArray 	src,                        /	input image.
						OutputArray 	dst,					/   output image of the same size and the same number of channels as src .
						int 	ddepth,							/   output image depth
						int 	dx,								/	order of the derivative x.				
						int 	dy,								/   order of the derivative y.
						int 	ksize = 3,						/   size of the extended Sobel kernel; it must be odd( 1, 3, 5, or 7)./-1 for sharr kernel
						double 	scale = 1,						/   optional scale factor for the computed derivative values; by default, no scaling is applied
						double 	delta = 0,						/   optional delta value that is added to the results prior to storing them in dst.
						int 	borderType = BORDER_DEFAULT		/   pixel extrapolation method
						)		

         dst=∂(x+y)src/∂x∂y
		 Mostly function is called for derivative x order and y order separetly ,after that combining them averaging or abs summing method.
         */

		convertScaleAbs(grad_x,abs_grad_x);//convert to CV_8U
		convertScaleAbs(grad_y,abs_grad_y);

		addWeighted(abs_grad_x,0.5,abs_grad_y,0.5,0,grad);//combining two derivatives by adding function/gamma=0

		imshow(window_name, grad);
		char key = (char)waitKey(0);
		if (key == 27)
		{
			break;
		}
		else if (key == 'K' || key == 'k')
		{
			ksize = ksize < 30 ? ksize + 2 : -1;//size equal -1 to use the sharr kernel
		}
		else if (key == 'S' || key == 's')
		{
			kscale++;
		}
		else if (key == 'D' || key == 'd')
		{
			delta++;
		}
		else if (key == 'R' || key == 'r')
		{
			ksize = -1;
			kscale = 1;
			delta = 0;
		}
	}

	return 0;
}