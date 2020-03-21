#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

static void help(char** argv)
{
	cout << endl
		<< argv[0] << " shows the usage of the OpenCV serialization functionality." << endl
		<< "usage: " << endl
		<< argv[0] << " outputfile.yml.gz" << endl
		<< "The output file may be either XML (xml) or YAML (yml/yaml). You can even compress it by "
		<< "specifying this in its extension like xml.gz yaml.gz etc... " << endl
		<< "With FileStorage you can serialize objects in OpenCV by using the << and >> operators" << endl
		<< "For example: - create a class and have it serialized" << endl
		<< "             - use it to read and write matrices." << endl;
}
class Mdata
{
public:
	Mdata():A(0),X(0),id()//default data
	{}
	explicit Mdata(int) :A(97), X(CV_PI), id("id_0") {}// explicit to avoid implicit conversion
	void write(FileStorage &fs)const
	{
		fs << "{" << "A" << A << "X" << X << "id" << id << "}";
	}
	void read(const FileNode & node)//to find in text,we are using node function
	{
		A = (int)node["A"];
		X = (double)node["X"];
		id = (string)node["id"];
	}
public:
	int A;
	double X;
	string id;
};
static void write(FileStorage &fs,const std::string&,const Mdata& x)//These write and read functions must be defined for the serialization in FileStorage to work
{
	x.write(fs);
}
static void read(const FileNode &Mnode,Mdata &x,const Mdata defaul=Mdata())//default value initilization/check node memory
{
	if (Mnode.empty())//check Mnode memory 
		x = defaul;
	else
		x.read(Mnode);
}
// This function will print our custom class to the console
static ostream& operator <<(ostream &out,const Mdata &x)//ostream overloading -using-->  ostream_object<<data_object
{
	out << "{id=" << x.id << ",";
	out << "X=" << x.X << ",";
	out << "A=" << x.A << "}" << endl;
	return out;
}
int main(int argc,char **argv)
{
	if(argc!=2)
	{
		help(argv);
		return -1;
	}
	string file = argv[1];

	{//write
		Mat R = Mat_<uchar>::eye(3, 3);
		//Mat T = Mat::zeros({2,2}, CV_8U);
		Mat T = Mat_<double>::zeros(3, 1);

		Mdata m(1);//Mdata=1; implicit conversion do not permit because of explicity
		//user defined data handled which has explicity
		FileStorage fs(file,FileStorage::WRITE);

		//text writing sequence
		fs << "iterationNr=" << 100;
		fs << "strings" << "[";  // text - string sequence
		fs << "image.jpg" << "aw" << "./image.jpg"; // close sequence
		fs << "]";

		fs << "Mapping";
		fs<< "{" << "One" << 1;
		fs << "Two" << 2 << "}";

		fs << "R" << R;     //cv:Mat
		fs << "T" << T;

		fs << "Mdata" << m;   //user data

		fs.release();//explicitly closing

		cout << "write done..." << endl;
	}
	{//read
		cout << endl << "Reading." << endl;
		FileStorage fs;
		fs.open(file, FileStorage::WRITE);

		int itNr;
		itNr = (int)fs["iterationNr"];//accesing map element
		cout << itNr;

		if (!fs.isOpened())
		{
			cerr << "error when file opening!!!";
			help(argv);
			return -1;
		}
		/*To read from these we use the cv::FileNode and the cv::FileNodeIterator data structures. 
		The [] operator of the cv::FileStorage class (or the getNode() function in Python) returns a cv::FileNode data type. 
		If the node is sequential we can use the cv::FileNodeIterator to iterate through the items.*/
		FileNode n = fs["strings"];//Read string sequence - get node
		if (n.type() != FileNode::SEQ)
		{
			cerr << "string is not a sequence!!!" << endl;
			return -1;
		}
		FileNodeIterator it = n.begin(),it_end = n.end();

		for (; it < it_end; it++)
			cout << (string)*it;

		n = fs["Mapping"]; //read mappings from sequence
		cout << "Two" << (int)(n["Two"]) << ";";
		cout << "One" << (int)(n["One"]) << endl << endl;

		Mdata m;
		Mat R, T;

		fs["R"] >> R;//read cv::Mat
		fs["T"] >> T;
		fs["Mdata"] >> m;//read own structure

		cout << endl << "R=" << R << endl;//uses internal write function
		cout << "T=" << T << endl<<endl;
		cout << "Mdata=" << endl<<m << endl << endl;

		/*
		 try out reading a non-existing read:
		 cout << "Attempt to read NonExisting (should initialize the data structure with its default).";
        fs["NonExisting"] >> m;
        cout << endl << "NonExisting = " << endl << m << endl;
		*/
	}
	cout << endl;
	cout << "open up "<<file<< "with a text editor to see the serialized data."<< endl;

	return 0;
}