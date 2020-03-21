#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <string.h>

using namespace cv;
using std::cout;
using std::endl;

Mat src, dst;

int morph_operation=0;
int morph_elem=0;
int morph_size=0;
int save_im=0;

const int max_elem=2;
const int max_operator = 4;
const int max_kernel_size=21;
const char* window_name = "Morphological Transformation";

void MorphologyOperations(int,void *);
void save_image(int, void*);
int main(int argc,char **argv)
{
	CommandLineParser pars(argc,argv,"{@input|image.jpg|image name}");
	src = imread(samples::findFile(pars.get<String>("@input")), IMREAD_COLOR);
	if(src.empty())
	{
		cout << "File couldn't open..." << endl;
		cout << "usage=" << argv[0] << " {image_name.<extention>}" << endl;
		return -1;
	}
	namedWindow(window_name,WINDOW_GUI_NORMAL);
	//trackbar for operations
	createTrackbar("Operator:\n 0: Opening - 1 : Closing  \n 2: Gradient - 3 : Top Hat \n 4: Black Hat\n",
		window_name,&morph_operation,max_operator,MorphologyOperations);
	//trackbar for elements
	createTrackbar("Element: \n 0:Rect - 1:Cross - 2:Ellipse ",window_name,&morph_elem,max_elem,MorphologyOperations);
	//trackbar for kernel size
	createTrackbar("Kernel Size:\n (2*n+1)",window_name,&morph_size,max_kernel_size,MorphologyOperations);
	//Every time we move any slider, the user's function Morphology_Operations will be called to effectuate a new morphology operation and
	//it will update the output image based on the current trackbar values.
	createTrackbar("Save:",window_name,&save_im,1,save_image);
	MorphologyOperations(0, 0);//default callback
	waitKey(0);
	return 0;
}
void MorphologyOperations(int ,void*)
{
	int operation = morph_operation + 2;//the values range from <2-6>, that is why we add (+2) 
	Mat element = getStructuringElement(morph_elem, Size(2 * morph_size + 1, 2 * morph_size + 1), Point(morph_size, morph_size));
	morphologyEx(src, dst, operation, element);
	if (save_im)save_image(0,0);
	/*
	src : Source (input) image
	dst: Output image
	operation: The kind of morphology transformation to be performed. Note that we have 5 alternatives:
		
	Opening: MORPH_OPEN : 2
	Closing: MORPH_CLOSE: 3
	Gradient: MORPH_GRADIENT: 4
	Top Hat: MORPH_TOPHAT: 5
	Black Hat: MORPH_BLACKHAT: 6
	*/
	imshow(window_name,dst);
}
void save_image(int, void*)
{
	 //new char[strlen(window_name) + 5];
	String dst_name = "./filter";
	std::stringstream ss;
	ss<< morph_operation;
	dst_name =dst_name+ ss.str()+".jpg";
	if (save_im) {
		try {
			imwrite(dst_name, dst);
		}
		catch (std::runtime_error &ex)
		{
			std::cerr << "exception while writing image"<<ex.what()<<endl;
			exit(EXIT_FAILURE);
		}
		catch(...)
		{
			std::cerr << "deafult exception" << endl;
			exit(EXIT_FAILURE);
		}
	}
}