#include <opencv2/opencv.hpp>
#include <iostream>
#include<string>

using namespace std;
using namespace cv;

Mat& scanImageAndReduceC(Mat& I,const uchar* const table);
Mat& scanImageAndReduceIterator(Mat&,const uchar* const table);
Mat& scanImageAndReduceRA(Mat&,const uchar * const table);
/*first const treats as pointer point a const value(const uchar tutan pointer, 
fakat point ettiği değer değişmez ama değişken değişebilir.)
second const use when we make adress constant which pointer use when point table.ex table adress const.
first const assure that we must assign matrix with const value and the second one assure that we cannot assign another 
different matrix after all in place of the table matrix so any mistake by assign will be obstructed.
*/
int main(int argc, char** argv)
{
	//PIXELS COLOR DEPTH REDUCTION 
	// Lookup tables are simple arrays (having one or more dimensions) that for a given input value variation holds the final output value
	int R;
	stringstream s;
	uchar table[256];
	Mat I;
	if (argc == 3)
		I = imread(argv[1], IMREAD_COLOR);
	else
		cout << "not enough parameter input!" << endl;

	if (I.empty())
	{
		cout << "The image" << argv[1] << " could not be loaded." << endl;
		return -1;
	}

	s << argv[2];
	s >> R;//stringstream use for convert thirdcommandline argument which is get reduction ratio to integer(we can use atoi instead)

	if(!s||!R)
	{
		cout << "invalid number entering for dividing!" << endl;
		return -1;
	}

	//LOOKUP table:its prevent redundant cpu usage 
	for (int i = 0; i < 256; i++)
		table[i] = (uchar)((i / R) * R);//for seeing all posibility, scannig all possible input values...
	
	const int times = 100;
	//TIME MEASUREMENT OF METHODS	
	//C METHOD--------------------------------------------------------------------------
	double t = (double)getTickCount();//hold starting count number
	//start C version reduction
	Mat M_clon = I.clone();
	for(int i=0;i<times;i++)//"times" times operation repeated...
	I=scanImageAndReduceC(M_clon,table);

	t =1000*(double)((getTickCount() - t) / getTickFrequency());//for miliseconds we multiply with 1000
	cout << "time<c>=" << t / times << "ms" << endl;
	//C METHOD END-----------------------------------------------------------------------

	//ITERATOR METHOD--------------------------------------------------------------------
	 t = (double)getTickCount();
	M_clon = I.clone();
	for (int i = 0; i < times; i++)
		I = scanImageAndReduceIterator(M_clon, table);

	t = 1000 * (double)((getTickCount() - t) / getTickFrequency());
	cout << "time<iterator>=" << t / times << "ms" << endl;
	//ITERATOR METHOD END---------------------------------------------------------------- 

	//On-The-Fly RA METHOD---------------------------------------------------------------
	 t = (double)getTickCount();
	M_clon = I.clone();
	for (int i = 0; i < times; i++)
		I = scanImageAndReduceIterator(M_clon, table);

	t = 1000 * (double)((getTickCount() - t) / getTickFrequency());
	cout << "time<RA>=" << t / times << "ms" << endl;
	//On-The-Fly RA METHOD END------------------------------------------------------------

	//last method is CORE function--------------------------------------------------------
	/*In İmage processing mostly that modifiying all image values to some other values with lookup table.
	Opencv provided some function for modifiying image data.We use the cv::LUT() function of the core module.
	First lookup table construct than function invoke after with parameter lookup table,input,output image variables.
	*/
	Mat J;
	Mat lookUpTable(1,256,CV_8U);//empty mat object
	uchar* p = lookUpTable.ptr();
	for (int i = 0; i < 255; i++)
		p[i] = table[i];
	/*strcpy(p,table);*/
	t = (double)getTickCount();
	for (int i = 0; i < times; i++)
		LUT(I, lookUpTable, J);

	t = 1000 * (double)((getTickCount() - t) / getTickFrequency());
	cout << "time<CORE>=" << t / times << "ms" << endl;
	//CORE FUNCTION END-----------------------------------------------------------------------------
	/*
	In conclude,when we use LUT function this is fastest way because of use intel multi_thread blocks.
	if we want to simple scan ,we simply use pointer method or we can use iterator method for safety with sacriface time. 
	And On-The-Fly RA is the most costly image scan mode.
	*/

	string im = "image(reduction=).jpg";
	im.insert(16, argv[2]);
	imwrite(im, J);

	namedWindow("original", WINDOW_AUTOSIZE);
	namedWindow("reduction filter", WINDOW_AUTOSIZE);

	imshow("original", I);
	imshow("reduction filter", J);

	cin >> t;

	return 0;
}
Mat& scanImageAndReduceC(Mat& I,const uchar* const table)
{
	CV_Assert(I.depth()==CV_8U);//assertion ensure is it uchar type
	int nChannel = I.channels();
	int nRows = I.rows;
	int nCol = (I.cols)*nChannel;

	if (I.isContinuous())
	{
		nRows *= nCol;
		nCol = 1;
	}

	int i, j;
	uchar* p;
	for (i = 0; i < nCol; i++)
	{
		p = I.ptr<uchar>(i);//satıra ptr işlevi ile ulaşabiliriz/dimension derecesi düşürüldü...
		for (j = 0; j < nRows; j++)
			p[j] = table[p[j]];//use with const instead of basic operations cpu usage efficiency increased...
	}
	return I;
	/*
	There is another way to do same operation.We can use data member of Mat object that return first indices pointer of matrix (0,0)
	İf there is no input image this will be Null.
	uchar * p=I.data;
	for(unsigned int i=0;i<nRows*nCol;++i)
	 *p++=table[*p];
	But this way will be lot harder to read on. 
	*/
}
Mat& scanImageAndReduceIterator(Mat& I,const uchar *const table)
{
	/*In case of the efficient way making sure that you pass through the right amount of 
    uchar fieldsand to skip the gaps that may occur between the rows was your responsibility.
	The iterator method is considered a safer way as it takes over these tasks from the user.
	*/
	CV_Assert(I.depth()==CV_8U);
	const int nChannel = I.channels();
	switch(nChannel)
	{
		case 1:
		{
			MatIterator_<uchar>it, end;//we need to end of iteration
			for (it = I.begin<uchar>(), end = I.end<uchar>(); it != end; ++it)
				*it = table[*it];
			break; }
		case 3:
		{
			MatIterator_<Vec3b>it, end;
			for (it = I.begin<Vec3b>(), end = I.end<Vec3b>(); it != end; ++it)
			{
				(*it)[0] = table[(*it)[0]];//it returns an 3 dimension array
				(*it)[1] = table[(*it)[1]];
				(*it)[2] = table[(*it)[2]];
			}
			break; }
	}
	/*
	in color images has three uchar item and to assure to make operation for all col we simply use <vec3b> 
	It's importatnt to remember that Opencv iterator module go through to columns and pass other channel column while  
	otomatically skip to next row(for Mat object so as Mat iterator all channel considered one row in one pixel).
	*/
	return I;
}
Mat& scanImageAndReduceRA(Mat& I, const uchar* const table)
{
	//same way as iterator and c versions but use internal at function for accessing matrix elements 
	/*
	The function takes your input type and coordinates and calculates the address of the queried item. 
	Then returns a reference to that. 
	This may be a constant when you get the value and non-constant when you set the value.
	(but cordinates must be valid)
	*/
	CV_Assert(I.depth()==CV_8U);
	const int nChannel = I.channels();
	switch (nChannel)
	{
	case 1:
		for (int i = 0; i < I.rows; i++)
			for (int j = 0; j < I.cols; j++)
				I.at<uchar>(i, j) = table[I.at<uchar>(i, j)];
		break;
	case 3:
		Mat_<Vec3b> _I = I;//Mat_ same as the Mat but you need specify the data type.we assign Mat to Mat_ 
		for(int i=0;i<I.rows;i++)
			for (int j = 0; j < I.cols; j++)
			{
				_I(i, j)[0] = table[_I(i, j)[0]];
				_I(i, j)[1] = table[_I(i, j)[1]];
				_I(i, j)[2] = table[_I(i, j)[2]];
			}
		I = _I;//we equalize data back 
		break;
	}
	return I;
}