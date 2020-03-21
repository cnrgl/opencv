#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

static void help(char** argv)
{
	cout << endl
		<< "This program demonstrated the use of the discrete Fourier transform (DFT). " << endl
		<< "The dft of an image is taken and it's power spectrum is displayed." << endl << endl
		<< "Usage:" << endl
		<< argv[0] << " [image_name -- default lena.jpg]" << endl << endl;
}
//image opt-->make template with two channel-->use dft -->turn back image to the one channel format -->find magnitude--> 
//-->turn magnitude into logaritmc scale -->visualization op.(quadratic swap and normalization)
int main(int argc,char **argv)
{

	help(argv);

	const char* file = argc > 1 ? argv[1] : "./lena.jpg";

	Mat I = imread(samples::findFile(file),IMREAD_GRAYSCALE);
	if (I.empty())
	{
		cout << "file could not open!!!" << endl;
		waitKey();
		return EXIT_FAILURE;
	}
	//The performance of a DFT is dependent of the image size.It tends to be the fastest for image sizes that are 
	//multiples of the numbers two, three and five.Therefore, to achieve maximal performance it is generally a good idea to
	//pad border values to the image to get a size with such traits. 
	//******for fastest code ,image size must be multiples 2 , 3 , 5****************************
	//OPTIMIZATION DFT SIZE-----------------------------------------------------------------
	Mat opt;
	int m = getOptimalDFTSize(I.rows);
	int n = getOptimalDFTSize(I.cols);
	copyMakeBorder(I, opt, 0, m - I.rows, 0, n - I.cols,BORDER_CONSTANT , Scalar::all(0));//add const (0) m-row times in bottom and n-cols time in right

	//The result of a Fourier Transform is complex. This implies that for each image value the result is two image values (one per component).
	//Moreover, the frequency domains range is much larger than its spatial counterpart. 

	//Make place for dtf components-------------------------------------------------------
	Mat planes[] = { Mat_<float>(opt),Mat::zeros(opt.size(),CV_32F) };//make plane for freq domain
	Mat complexI;
	merge(planes, 2, complexI);//by using mat planes parts ,convert second domain to channel coordinates
	//one channel for real part ; second channel for imaginary part i-->ch1 j-->ch2

	//The function cv::merge merges several arrays to make a single multi-channel array. 
	//That is, each element of the output array will be a concatenation of the elements of the input arrays,
	//where elements of i-th input array are treated as mv[i].channels()-element vectors.


	//DFT---------------------------------------------------------------------------------
	dft(complexI, complexI);//distiance is same with input/dft needs two channel array for grayscale

	//FIND MAGNITUDE ---------------------------------------------------------------------
	split(complexI,planes);//complexI --> planes single channel array
	//planes[0]=(Re(DFT(I)),planes[1]=(Im(DFT(I))) 
	magnitude(planes[0],planes[1],planes[0]);//planes[0]=Magnitude(I)
	Mat mag = planes[0];
	//LOGARITMIC SCALE TRANSFORM----------------------------------------------------------
	/*It turns out that the dynamic range of the Fourier coefficients is too large to be displayed on the screen.
	We have some small and some high changing values that we can't observe like this.
	We should transform DFT coefficents into logaritmic scale M1=log(1+M)
	Therefore the high values will all turn out as white points, while the small ones as black.
	*/
	mag += Scalar::all(1);//To use logaritmic scale,we add 1 in every pixel values
	log(mag, mag);// log(1 + sqrt(Re(DFT(I)) ^ 2 + Im(DFT(I)) ^ 2))

	//REARRANGE image - find quadrants-----------------------------------------------------
	//For visualization purposes we may also rearrange the quadrants of the result, so that the origin(zero, zero) corresponds with the image center.

	mag = mag(Rect(0,0,mag.cols & -2,mag.rows & -2));
	//crop spectrum if it has odd number elemnts/!(row&2) =row & -2 -->simply get high for all bit except first bit
	//above image will change by quadrants regions

	// rearrange the quadrants of Fourier image  so that the origin is at the image center
	int cx = mag.cols / 2;
	int cy = mag.rows / 2;

	Mat q0(mag,Rect(0,0,cx,cy));//Top-Left - ROI - not copying image matrix
	Mat q1(mag,Rect(cx,0,cx,cy));//Top-right
	Mat q2(mag,Rect(0,cy,cx,cy));//bottom-left
	Mat q3(mag,Rect(cx,cy,cx,cy));//bottom-right

	Mat tmp;
	q0.copyTo(tmp);// swap quadrants (Top-Left with Bottom-Right)--1/roi
	q3.copyTo(q0);
	tmp.copyTo(q3);

	q2.copyTo(tmp);// swap quadrant (Top-Right with Bottom-Left)
	q1.copyTo(q2);
	tmp.copyTo(q1);

	//NORMALIZE..........................................................
	//We now have the magnitudes, however this are still out of our image display range of zero to one.We normalize magnitudes
	normalize(mag,mag,0,1,NORM_MINMAX);//normalize to float values between 0 and 1 

	/*You can see that the most influential components of the frequency domain (brightest dots on the magnitude image) follow 
	the geometric rotation of objects on the image.From this we may calculate the offset and perform an image rotation to 
	correct eventual miss alignments.*/

	imshow("input image",I);
	imshow("DFT of image",mag);
	waitKey();
	return 0;
}