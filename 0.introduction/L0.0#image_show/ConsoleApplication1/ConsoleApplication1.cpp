#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
using namespace cv;
using namespace std;
int main(int argc, char** argv)
{
	String imageName("image.jpg"); // by default
	if (argc > 1)
	{
		imageName = argv[1];
	}
	Mat image;
	image = imread(samples::findFile(imageName), IMREAD_COLOR); // Read the file
	/*
	IMREAD_UNCHANGED (<0) loads the image as is (including the alpha channel if present)
IMREAD_GRAYSCALE ( 0) loads the image as an intensity one
IMREAD_COLOR (>0) loads the image in the RGB format
*/
	if (image.empty())                      // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}
	namedWindow("Display window",WINDOW_AUTOSIZE|WINDOW_FREERATIO); // Create a window for display WINDOW_NORMAL OR (WINDOW_KEEPRATIO) or not (WINDOW_FREERATIO).
	imshow("Display window", image);                // Show our image inside it.
	waitKey(0); // Wait for a keystroke in the window
	return 0;
}