/*
 *
 * Produce the dividing command lines
 *
 */



#include "cv.h"
#include "highgui.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <math.h>
#include <fstream>
using namespace std;

int main(int argc, char *argv[])
{
	cout << "argv[1] = "<< argv[1] << endl;
	cout << "argv[2] = "<< argv[2] << endl;

	const char *PathName1 = argv[1];	/////////////////////////////   Input path name    should be changed here

    const char *ImageName1 = argv[2];
    const char *FormatName1 = "_x2.5_z0.tif";
    char FullImageName1[100];
    strcpy(FullImageName1, PathName1);
    strcat(FullImageName1, ImageName1);
    strcat(FullImageName1, FormatName1);

    cout << "FullImageName1 = "<< FullImageName1 << endl;

	IplImage* src = cvLoadImage( FullImageName1, 1 );
	int ImageW = 1920;
	int ImageH = 1080;
    int width = (src->width)*16/ImageW+1;
    int height = (src->height)*16/ImageH+1;

    int TotalNum = width*height;

    cout << "width = "<< width << endl;
    cout << "height = "<< height << endl;
    cout << "TotalNum = "<< TotalNum << endl;

    const char *TextName1 = argv[3];
    char FullTextName1[100];
    strcpy(FullTextName1, PathName1);
    strcat(FullTextName1, TextName1);

    cout << "FullTextName1 = "<< FullTextName1 << endl;





    const char *FormatName2 = ".ndpi";
    char OutputImageName1[100];
    strcpy(OutputImageName1, ImageName1);
    strcat(OutputImageName1, FormatName2);

    cout << "OutputImageName1 = "<< OutputImageName1 << endl;



    ifstream infile;
    ofstream outfile0;
    infile.open(FullTextName1);                    //test.txt should be changed, when rewrite, the non-rewrite part will not be deleted
    if(infile.fail ())
    {
    outfile0.open(FullTextName1);				///////////////////////////////////////////  should change here
    };

    ofstream outfile(FullTextName1,ios::in);		///////////////////////////////////////////  should change here
    if(!outfile)
    {
    cerr<<"open SplitRun.txt error!\n";
    return 0;
    }

    for (int i = 0;i<TotalNum;i++)
    {
    	int row = i/width;
    	int col = i%width;

    	outfile<<"./ndpisplit -Ex40,z0,"<<col*ImageW<<","<<row*ImageH<<","<<ImageW<<","<<ImageH<<","<<"result"<<i+1<<" "<<"-MJ"<<" "<<OutputImageName1<<endl;		///////////////////////////////////////////  should change here

    }

    outfile.close();




    const char *TextName2 = argv[4];
    char FullTextName2[100];
    strcpy(FullTextName2, PathName1);
    strcat(FullTextName2, TextName2);

    cout << "FullTextName2 = "<< FullTextName2 << endl;


    char InputImageName1[100];
    strcpy(InputImageName1, PathName1);
    strcat(InputImageName1, ImageName1);

    cout << "InputImageName1 = "<< InputImageName1 << endl;



    int step = atoi(argv[9]);
    cout << "step = "<< step << endl;

    int RowNum = (height-2)*step+1;
    int ColNum = (width-2)*step+1;
    int TotalNumBig = RowNum*ColNum;

    cout << "RowNum = "<< RowNum << endl;
    cout << "ColNum = "<< ColNum << endl;
    cout << "TotalNumBig = "<< TotalNumBig << endl;


    ifstream infile1;
    ofstream outfile1;
    infile1.open(FullTextName2);           //q1.txt should be changed, when rewrite, the non-rewrite part will not be deleted
    if(infile1.fail ())
    {
    outfile1.open(FullTextName2);				///////////////////////////////////////////  should change here
    };

    ofstream outfile2(FullTextName2,ios::in);		///////////////////////////////////////////  should change here
    if(!outfile2)
    {
    cerr<<"open RatioRun.txt error!\n";
    return 0;
    }


    //for (int i = 0;i<TotalNum;i++)
    for (int i = 0;i<TotalNumBig;i++)
    {
    	//outfile2<<"source_images="<<InputImageName1<<"_x40_z0_result"<<i+1<<"_i1j1.jpg "<<endl;		///////////////////////////////////////////  should change here
    	outfile2<<"source_images="<<PathName1<<i+1<<".jpg "<<endl;		///////////////////////////////////////////  should change here

    }


    outfile2.close();


    const char *PathName2 = argv[5];
    const char *TextName3 = argv[6];
    char FullTextName3[100];
    strcpy(FullTextName3, PathName2);
    strcat(FullTextName3, TextName3);

    cout << "FullTextName3 = "<< FullTextName3 << endl;



    const char *TextName4 = argv[7];    //  this is for the location of saving the RatioResult (RatioResult.txt), which is used in macro DAB4.txt
    char FullTextName4[100];
    strcpy(FullTextName4, PathName1);
    strcat(FullTextName4, TextName4);

    cout << "FullTextName4 = "<< FullTextName4 << endl;






    //  this is for macro DAB4.txt

    ifstream infile3;
    ofstream outfile3;
    infile3.open(FullTextName3);           //q1.txt should be changed, when rewrite, the non-rewrite part will not be deleted
    if(infile3.fail ())
    {
    outfile3.open(FullTextName3);				///////////////////////////////////////////  should change here
    };

    ofstream outfile4(FullTextName3,ios::in);		///////////////////////////////////////////  should change here
    if(!outfile4)
    {
    cerr<<"open CalculateRatio.txt error!\n";
    return 0;
    }


    outfile4<<"filestring=File.openAsString(\""<<FullTextName2<<"\");"<<endl;
    outfile4<<"rows=split(filestring, \"\\n\");"<<endl;
    outfile4<<"for(i=0; i<rows.length; i++)"<<endl;
    outfile4<<"	{"<<endl;
    outfile4<<"		columns=split(rows[i],\"\\t\");"<<endl;
    outfile4<<"		print(rows[i]);"<<endl;
    outfile4<<"		run("<<endl;
    outfile4<<"			\"ImmunoRatio \","<<endl;
    outfile4<<"			rows[i] +"<<endl;
    outfile4<<"			\"blankfield_image= \" +"<<endl;
    outfile4<<"			\"log_file="<<FullTextName4<<" \" +"<<endl;
    outfile4<<"			\"result_image_directory= \" +"<<endl;				//////// If don't need result image, use this
    //outfile4<<"			\"result_image_directory="<<PathName1<<" \" +"<<endl;   //////// If need result image, use this
    outfile4<<"			\"source_image_copy_directory= \" +"<<endl;
    outfile4<<"			\"brown_threshold_adjustment=0 \" +"<<endl;
    outfile4<<"			\"blue_threshold_adjustment=0 \" +"<<endl;
    outfile4<<"			\"scale_pixels_per_um=4.00 \" +"<<endl;
    outfile4<<"			\"result_correction_coefficient_a=0.00006442 \" +"<<endl;
    outfile4<<"			\"result_correction_coefficient_b=-0.0019840 \" +"<<endl;
    outfile4<<"			\"result_correction_coefficient_c=0.61100000 \" +"<<endl;
    outfile4<<"			\"result_correction_coefficient_d=0.43210000 \" +"<<endl;
    outfile4<<"			\"sample_identifier=1.0 \""<<endl;
    outfile4<<"		);"<<endl;
    outfile4<<"	}"<<endl;



    outfile4.close();






    const char *TextName5 = argv[8];
    char FullTextName5[100];
    strcpy(FullTextName5, PathName1);
    strcat(FullTextName5, TextName5);

    cout << "FullTextName5 = "<< FullTextName5 << endl;





    ifstream infile5;
    ofstream outfile5;
    infile5.open(FullTextName5);           //q1.txt should be changed, when rewrite, the non-rewrite part will not be deleted
    if(infile5.fail ())
    {
    outfile5.open(FullTextName5);				///////////////////////////////////////////  should change here
    };

    ofstream outfile6(FullTextName5,ios::in);		///////////////////////////////////////////  should change here
    if(!outfile6)
    {
    cerr<<"open WidthHeight.txt error!\n";
    return 0;
    }


    outfile6<<width<<endl;
    outfile6<<height<<endl;

    outfile6.close();




    return 0;









}







