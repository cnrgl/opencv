#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	cout << "\n Zoom In-Out demo \n "
		"------------------  \n"
		" * [i] -> Zoom in   \n"
		" * [o] -> Zoom out  \n"
		" * [ESC] -> Close program \n" << endl;
	const char* filename = (argc >= 2) ? argv[1] : "image.jpg";
	Mat src = imread(samples::findFile(filename),IMREAD_COLOR);

	if (src.empty())
	{
		cout << "error when file loading..." << endl;
		cout << "usage:"<<argv[0]<<" image_name"<< endl;
		return EXIT_FAILURE;
	}
	cout << "row=" << src.rows << "col=" << src.cols << endl;
	for (;;)
	{
		imshow("image", src);
		char c=(char)waitKey(0);//it returns pressed key or -1 if no key pressed.
		if (c == 27)//escape
		{
			break;
		}
		else if (c == 'i')
		{//upsamplink==zoom image
			pyrUp(src,src,Size(src.cols/2,src.rows/2));
			cout << "zoom in image: 2x " << endl;
		}
		else if(c=='o')
		{
			pyrDown(src,src,Size(src.cols*2,src.rows*2));
			cout << "zoom out image: 2/" << endl;
		}
	}
	//Notice that it is important that the input image can be divided by a factor of two (in both dimensions). Otherwise, an error will be shown.
	return 0;
}