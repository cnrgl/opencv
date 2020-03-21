#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
// we're NOT "using namespace std;" here, to avoid collisions between the beta variable and std::beta in c++17
using std::cin;
using std::cout;
using std::endl;
using namespace cv;

int main(int argc,char ** argv)
{
	CommandLineParser parser(argc,argv,"{@input | lena.jpg |input image}");//3 and above parameter -->  "{parameter name|default value if empty is empty|help message}""{other key} "
	Mat image = imread(samples::findFile(parser.get<String>("@input")));//parser.get-->find parameter
	if (image.empty()) {
		cout << "image couldnt loaded!" << endl;
		cout << "Usage: " << argv[0] << " <Input image>" << endl;
		waitKey();
		return EXIT_FAILURE;
	}

	//we need to new empty image template
	Mat new_image = Mat::zeros(image.size(),image.type());
	double alpha = 1.0;//constrast adjusting control
	double beta = 0.0;//brightness control
	double gamma = 1;//gamma correction control

	cout << "Basic Lineer Transformation" <<endl;
	cout << "---------------------------" << endl;
	cout << "Enter alpha value[1.0-3.0]=" << endl; cin >> alpha;
	if (alpha < 0) { cout << "alpha must be pozitive!" << endl; waitKey();  return EXIT_FAILURE; }
	cout << "Enter beta value[-100-100]" << endl; cin >> beta;
	cout << "enter gamma value[0.01-50]= " << endl; cin >> gamma;

	
	for (int x = 0; x < image.rows; x++)
		for (int y = 0; y < image.cols; y++)
			for (int c = 0; c < image.channels(); c++)
				new_image.at<Vec3b>(x, y)[c] = saturate_cast<uchar>(alpha*image.at<Vec3b>(x,y)[c]+beta);

	//for make sure values is not in out of range we simply use saturate cast which also check values whether in saturation range(0-255)
	//image.convertTo(new_image, -1, alpha, beta); -->is make same operation more efficiently(because of optimization) so in following codes we use just this

	imshow("original image",image);
	imshow("new image",new_image);

	Mat lookup(1,256,CV_8U);
	uchar* p = lookup.ptr();

	for (int i = 0; i < 255; i++)
		p[i] = saturate_cast<uchar>(pow(i/255.0,gamma)*255.0);
	
	Mat out = image.clone();
	LUT(image,lookup,out);

	imshow("new image(gamma)", out);

	waitKey();
	return EXIT_SUCCESS;
}