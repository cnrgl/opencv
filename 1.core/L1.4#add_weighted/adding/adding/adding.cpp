#include<opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
//avoid confision beta keyword in std and cv namespace we add std's keywords additionally
using std::cout;
using std::cin;
using std::endl;

int main(void)
{
	Mat im_back, im_front, out;
	double alpha=0.5, beta,input,gamma=0.0;
	cout << "input weight ratio[0-1]=" << endl;
	cin >> input;

	if ((input < 1) && (input > 0))
		alpha = input;
	    beta = 1 - alpha;

	im_back = imread(samples::findFile("./image_back.jpg"));
	im_front = imread(samples::findFile("./image_front.png"));
	
	if (im_back.empty()) { cout << "back ground couldnt read!" << endl; return EXIT_FAILURE; }
	if (im_front.empty()) { cout << "front image couldnt read!" << endl; return EXIT_FAILURE; }

	addWeighted(im_back,alpha,im_front,beta,gamma,out);

	namedWindow("weighted", WINDOW_AUTOSIZE);
	imshow("weighted", out);

	waitKey();

	return 0;
}