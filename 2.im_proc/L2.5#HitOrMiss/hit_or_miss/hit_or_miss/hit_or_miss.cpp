#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>

using namespace cv;
//The Hit-or-Miss transformation is useful to find patterns in binary images. In particular, it finds those pixels whose neighbourhood matches 
//the shape of a first structuring element B1 while not matching the shape of a second structuring element B2 at the same time.
int main()
{
	Mat input_image = (Mat_<uchar>(8,8) << 
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 255, 255, 255, 0, 0, 0, 255,
		0, 255, 255, 255, 0, 0, 0, 0,
		0, 255, 255, 255, 0, 255, 0, 0,
		0, 0, 255, 0, 0, 0, 0, 0,
		0, 0, 255, 0, 0, 255, 255, 0,
		0, 255, 0, 255, 0, 0, 255, 0,
		0, 255, 255, 255, 0, 0, 0, 0); 
	Mat kernel = (Mat_<int>(3, 3) <<
		0,  1, 0,
		1, -1, 1,
		0,  1, 0 );
	Mat output_image;
	morphologyEx(input_image,output_image,MORPH_HITMISS,kernel);
	
	const char rate = 50;
	kernel = (kernel + 1) * 127;//uchar convertion
	kernel.convertTo(kernel, CV_8U);

	resize(kernel, kernel, Size(), rate, rate,INTER_NEAREST);
	/*
	resize function apply upsampling/interpolation operation-----

	src  -->	input image.
	dst	 -->output image; it has the size dsize (when it is non-zero) or the size computed from src.size(), fx, and fy; the type of dst is the same as of src.
	dsize-->	output image size; if it equals zero, it is computed as:
	            dsize = Size(round(fx*src.cols), round(fy*src.rows))
				Either dsize or both fx and fy must be non-zero.
	fx	-->scale factor along the horizontal axis; when it equals 0, it is computed as
				(double)dsize.width/src.cols
	fy	-->scale factor along the vertical axis; when it equals 0, it is computed as
				(double)dsize.height/src.rows
    interpolation -->it will generally look best with INTER_AREA interpolation, whereas to enlarge an image, 
	       it will generally look best with c::INTER_CUBIC (slow) or INTER_LINEAR (faster but still looks OK).
				*/
	imshow("kernel",kernel);
	moveWindow("kernel",0,0);

	resize(input_image, input_image, Size(), rate, rate, INTER_NEAREST);
	imshow("input_image",input_image);
	moveWindow("input_image",0,200);

	resize(output_image, output_image, Size(), rate, rate, INTER_NEAREST);
	imshow("output_image", output_image);
	moveWindow("output_image",500,200);

	waitKey(0);
	return 0;
}