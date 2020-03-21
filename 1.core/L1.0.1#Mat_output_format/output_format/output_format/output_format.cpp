#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
	Mat M = Mat(3, 3, CV_8UC3);
	M = (Mat_<double>(3, 3) << 0, -2, 3, 5, 7, 9, 1, 0, -1);
	
	//formatted outputs:
	cout << " M<default>=" << endl << M << endl << endl;//default
	cout << " M<python>=" << endl << format(M, Formatter::FMT_PYTHON) << endl << endl;//python
	cout << "R (csv)     = " << endl << format(M,Formatter::FMT_CSV) << endl << endl;//csv
	cout << "R (numpy)   = " << endl << format(M,Formatter::FMT_NUMPY) << endl << endl;//numpy
	cout << "R (c)       = " << endl << format(M,Formatter::FMT_C) << endl << endl;//c
	
	//other output items::
	
	Point2f P(5, 1);
	cout << "Point (2D) = " << P << endl << endl;

	Point3f P3f(2, 6, 7);
	cout << "Point (3D) = " << P3f << endl << endl;

	vector<float> c;//std::vector
	c.push_back((float)CV_PI); c.push_back(3); c.push_back(3.01f);
	cout<< "Vector of floats via Mat = " << Mat(c)<< endl << endl;

	vector<Point2f> d(20);

	for (size_t i = 0; i < d.size(); ++i) 
		d[i] = Point2f((float)(i * 5), (float)(i % 7));
	
	
	cout << "A vector of 2D Points = " << d<< endl << endl;
	int g;
	cin >>g ;
	waitKey(0);

	return 0;
}