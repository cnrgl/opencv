#include <opencv2/opencv.hpp>

using namespace cv;
int main(int argc, char** argv)
{
	
	char* imageName = argv[1];//get image from console
	Mat image;
	
	image = imread(imageName, IMREAD_COLOR);
	if (argc != 2 || !image.data)
	{
		printf(" No image data \n ");
		return -1;
	}
	Mat gray_image;
	
	cvtColor(image, gray_image, COLOR_BGR2GRAY);//change image from bgr to one channel colouring
	imwrite("./Gray_Image.jpg", gray_image);//image which converted to gray is written in filesystem 

	namedWindow(imageName, WINDOW_AUTOSIZE);//using 2 windows to show our images
	namedWindow("Gray image", WINDOW_AUTOSIZE);
	
	imshow(imageName, image);
	imshow("Gray image", gray_image);
	
	waitKey(0);
	return 0;
}