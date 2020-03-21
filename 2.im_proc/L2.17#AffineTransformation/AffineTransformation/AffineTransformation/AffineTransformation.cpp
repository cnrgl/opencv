#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
/*Affine transform is kind of transfer function which is transform src image to dst image in meaning. 
  A transformation that can be expressed in the form of a matrix multiplication (linear transformation) followed by a vector addition (translation).
  dst image show as src image and affine matrix(or relation or tf matrix) combining.
  *Relation between two image is handle with 2x3 matrix which first two coulumn are scaling and rotation const and third column is  vector addition-translation(contrast or bias or const adding)
  *Relation matrix is also consider as mapping operator which is use both two dimension(for just image processing) and translation const to obtain one dimension of distiance map.  
    T=A⋅[xy]T+B  or T=M⋅[x,y,1]T  (T is distiance matrix , B is translation matrix ,A is transforming matrix)
	M=[AB]=[a00 a01 b00; a10 a11 b10]2×3

	T=[a00x+a01y+b00 ; a10x+a11y+b10]2x1
  */
using namespace std;
using namespace cv;

int main(int argc,char **argv)
{
	//Applies an Affine Transform to the image. This transform is obtained from the relation between three points. We use the function cv::warpAffine for that purpose.
	CommandLineParser parser(argc, argv, "{@input | image.jpg | input image}");
	Mat src = imread(samples::findFile(parser.get<String>("@input")));
	if (src.empty())
	{
		cout << "Could not open or find the image!\n" << endl;
		cout << "Usage: " << argv[0] << " <Input image>" << endl;
		return -1;
	}

	Point2f srcTri[3];
	srcTri[0] = Point2f(0.f,0.f);
	srcTri[1] = Point2f(src.cols-1.f,0.f);
	srcTri[2] = Point2f(0.f,src.rows-1.f);
	Point2f dstTri[3];
	dstTri[0] = Point2f(0.f,src.rows*0.33f);
	dstTri[1] = Point2f(src.cols*0.85f,src.rows*0.25f);
	dstTri[2] = Point2f(src.cols*0.15f,src.rows*0.7f);

	//DEBUG
	Mat points;
	const Point* p1[1] = { (Point*)srcTri };
	const Point* p2[1] = { (Point*)dstTri };
	int ntp[] = { 3 };
	fillPoly(points, p1, ntp, 1, Scalar(0, 0, 225), LINE_8);
	fillPoly(points, p2, ntp, 1, Scalar(225, 0, 0), LINE_8);
	imshow("Shapes",points);
	//DEBUG_END
	Mat warp_mat = getAffineTransform(srcTri,dstTri);//getting transformation relation

	/*
	Mat cv::getAffineTransform	(	const Point2f 	src[],    //Coordinates of triangle vertices in the source image.
                                    const Point2f 	dst[]     //Coordinates of the corresponding triangle vertices in the destination image.
                                 )		
	*/
	Mat warp_dst = Mat::zeros(src.rows,src.cols,src.type());//apply transformation to all image
	warpAffine(src,warp_dst,warp_mat,warp_mat.size());
	/*
	warpAffine	(	InputArray 	src,                                   //input image.
					OutputArray 	dst,                               //output image that has the size dsize and the same type as src .
					InputArray 	M,                                     //2×3  transformation matrix.
					Size 	dsize,                                     //size of the output image.
					int 	flags = INTER_LINEAR,                      //combination of interpolation methods (see InterpolationFlags) and the optional flag WARP_INVERSE_MAP that means that M is the inverse transformation ( dst→src ).
					int 	borderMode = BORDER_CONSTANT,              //pixel extrapolation method (see BorderTypes); when borderMode=BORDER_TRANSPARENT, it means that the pixels in the destination image corresponding to the "outliers" in the source image are not modified by the function.
					const Scalar & 	borderValue = Scalar()             //value used in case of a constant border; by default, it is 0.
					)

					dst(x,y)=src(M11x+M12y+M13,M21x+M22y+M23)
			when the flag WARP_INVERSE_MAP is set. Otherwise, the transformation is first inverted with invertAffineTransform and then put in the formula above instead of M.
			The function cannot operate in-place.
	*/
	//ROTATİON
	Point center = Point(warp_dst.cols/2,warp_dst.rows/2);
	double angle = -50.0;
	double scale = 0.6;

	Mat rot_mat = getRotationMatrix2D(center,angle,scale);//works like getAffineTransform but ability is restricted for just rotation
	Mat warp_rotate_dst;
	warpAffine(src, warp_rotate_dst, rot_mat, warp_dst.size());

	/*
	getRotationMatrix2D	(	Point2f 	center,        //Center of the rotation in the source image.
							double 	angle,			   //Rotation angle in degrees. Positive values mean counter-clockwise rotation (the coordinate origin is assumed to be the top-left corner).
							double 	scale			   //Isotropic scale factor.
						)	
	The transformation maps the rotation center to itself. If this is not the target, adjust the shift.
	*/
	if (warp_dst.empty())
	{

		cout << "dst is empty" << endl;
			return -1;
	}
	imshow("Source Image",src);
	imshow("Warp",warp_dst);
	imshow("Warp+Rotation",warp_rotate_dst);

	waitKey();
	return 0;
}
