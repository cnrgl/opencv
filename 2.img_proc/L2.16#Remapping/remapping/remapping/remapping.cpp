#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

const char* remapwindow = "Remapping";
using namespace cv;

void update_map(int &ind,Mat &x_map,Mat &y_map);
int main(int argc,char **argv)
{
	CommandLineParser pars(argc,argv,"{@image|image_astronauts.jpg|input image}");
	std::string file_name =pars.get<std::string>(0);
	Mat src = imread(samples::findFile(file_name),IMREAD_COLOR);

	if (src.empty())
	{
		std::cout<<"image couldnt load."<<std::endl;
		std::cout << "Usage:"<<argv[0]<<"image_name"<<std::endl;
		return EXIT_FAILURE;
	}

	Mat dst(src.size(),src.type());
	Mat x_map(src.size(),CV_32FC1);
	Mat y_map(src.size(),CV_32FC1);

	namedWindow(remapwindow,WINDOW_AUTOSIZE);
	int ind=0;
	for (;;)
	{
		std::cout << "map updated" << std::endl;
		update_map(ind,x_map,y_map);//function will define two mapping function for x and y direction independently each other than assign into x_map and  y_map
		remap(src,dst,x_map,y_map,INTER_LINEAR,BORDER_CONSTANT,Scalar(0,0,0));
		/*
		This function perform remapping by using xmap((x,y)--to--(Xnew)) and ymap whose are both function of old x and y .also function apply interpolation in pixels area which is  not exist.
		Second map existence is not obligatory in case map1 take both (x,y) values as a parameter,
		dst(x,y)=src(mapx(x,y),mapy(x,y))
		The reason you might want to convert from floating to fixed-point representations of a map is that they can yield much faster (2x) remapping operations.
		In case destion size isnt match with src size,interpolation or extrapolation could apply.

		remap	(	InputArray 	src,                            //Source image
					OutputArray 	dst,                        //Destination image. It has the same size as map1 and the same type as src .
					InputArray 	map1,                           //The first map of either (x,y) points or just x values having the type CV_16SC2 , CV_32FC1, or CV_32FC2.
					InputArray 	map2,                           //The second map of y values having the type CV_16UC1, CV_32FC1, or none (empty map if map1 is (x,y) points), respectively.
					int 	interpolation,                      //Interpolation method (see InterpolationFlags). The method INTER_AREA is not supported by this function.
					int 	borderMode = BORDER_CONSTANT,       //Pixel extrapolation method (see BorderTypes). When borderMode=BORDER_TRANSPARENT, it means that the pixels in the destination image that corresponds to the "outliers" in the source image are not modified by the function.
					const Scalar & 	borderValue = Scalar()      //Value used in case of a constant border. By default, it is 0.
				)	
					Due to current implementation limitations the size of an input and output images should be less than 32767x32767.
		*/
		imshow(remapwindow,dst);
		char c = (char)waitKey(1000);//wait for 1000 ms
		if(c==27)
			{
			break;
		}
	}
	return 0;
}
void update_map(int &ind,Mat &x_map,Mat &y_map)//mapping scheme function
{
	//We are going to perform 4 different mappings :
	
	for(int i=0;i < x_map.rows; i++)
		for (int j=0; j < x_map.cols; j++)
		{
			switch (ind)
			{
			case 0://Reduce the picture to half its size and will display it in the middle:
				if ((i<0.75f*x_map.rows)&&(i>0.25f*x_map.rows)&&(j<0.75f*x_map.cols)&&(j>0.25f*x_map.cols))
				{
					x_map.at<float>(i, j) = (float)(2 * j - 0.5f * x_map.cols) + 0.5f;//multiply with two compress half size of image.
					y_map.at<float>(i, j) = (float)(2 * i - 0.5f * x_map.rows) + 0.5f;
				}
				else
				{
					x_map.at<float>(i, j) = 0;
					y_map.at<float>(i, j) = 0;
				}
				break;
			case 1://Turn the image upside down: h(i,j)=(i,src.rows−j)
				x_map.at<float>(i, j) = (float)j;
				y_map.at<float>(i, j) = (float)x_map.rows-i;//revert and shift right with rows times
				break;
			case 2://Reflect the image from left to right: h(i,j)=(src.cols−i,j)
				x_map.at<float>(i, j) = (float)x_map.cols - j;
				y_map.at<float>(i, j) = (float)i;
				break;
			case 3://Combination of b and c: h(i,j)=(src.cols−i,src.rows−j)
				x_map.at<float>(i, j) = (float)x_map.cols - j;
				y_map.at<float>(i, j) = (float)x_map.rows - i;
				break;
			default:
				break;
			}
		}
	ind = (ind+1) % 4;
}
