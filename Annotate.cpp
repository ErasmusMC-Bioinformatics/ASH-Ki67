/*
 *
 * CutPNG
 *
 */




#include "cv.h"
#include "highgui.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <math.h>
#include <fstream>
#include <sstream>
using namespace std;
//#include "string.h"


int main(int argc, char *argv[])
{
	cout << "argv[1] = "<< argv[1] << endl;
	cout << "argv[2] = "<< argv[2] << endl;


	const char *PathName1 = argv[1];	/////////////////////////////   Input path name    should be changed here

    const char *ImageName1 = argv[3];
    const char *FormatName1 = "_x2.5_z0.tif";
    char FullImageName1[100];
    strcpy(FullImageName1, PathName1);
    strcat(FullImageName1, ImageName1);
    strcat(FullImageName1, FormatName1);

    cout << "FullImageName1 = "<< FullImageName1 << endl;

	IplImage* src = cvLoadImage( FullImageName1, 1 ); ///////      This is the input 2  "DAB"      //////   should be changed here

	int ImageW = 1920;					////////////////////////////////////////////////////////////   should be changed here
	int ImageH = 1080;					////////////////////////////////////////////////////////////   should be changed here
    int width = (src->width)*16/ImageW+1;
    int height = (src->height)*16/ImageH+1;

    int step = atoi(argv[4]);
    cout << "step = "<< step << endl;

    int RowNum = (height-2)*step+1;
    int ColNum = (width-2)*step+1;
    int TotalNumBig = RowNum*ColNum;

    cout << "RowNum = "<< RowNum << endl;
    cout << "ColNum = "<< ColNum << endl;
    cout << "TotalNumBig = "<< TotalNumBig << endl;


    const char *TextName1 = argv[2];
    char FullTextName1[100];
    strcpy(FullTextName1, PathName1);
    strcat(FullTextName1, TextName1);

    cout << "FullTextName1 = "<< FullTextName1 << endl;

    fstream file;
    file.open(FullTextName1,ios::in);   		///        This is the input 1           ////  should be changed here

    if(!file)
    cout<<"file not founded"<<endl;

    int Lengthb = 6000*step*step;   // the image number is about 16000, so I use 24000
    int Lengtha = Lengthb * 2;

    float a[Lengtha];
    float b[Lengthb];

    int indexb[Lengthb];
    for(int i = 0;i<Lengthb;i++)
    	indexb[i]=i+1;

    for(int i = 0;i<Lengtha;i++)
    	a[i]=0.0;
    for(int i = 0;i<Lengthb;i++)
    	b[i]=0.0;

    int pos = 0;
    while(!file.eof())//是否到文件结尾
    {
     file>>a[pos];
     pos++;
     if(pos>=Lengtha)
      break;
    }
    file.close();
    for(int i = 0;i<Lengthb;i++)
    	b[i]=a[2*i+1];


    //////// no-use1
    char TestImageName[100];
    const char *Postfix = "_x40_z0_result";
    strcpy(TestImageName, ImageName1);
    strcat(TestImageName, Postfix);
    cout << "TestImageName = "<< TestImageName << endl;
    //////// no-use1 ends here


    //for (int i = 0;i<Total;i++)
    for (int i = 0;i<TotalNumBig;i++)
    {
    	stringstream ss;
    	ss<<i+1<<".jpg";
        string filename = ss.str();

        const char *ImageName2 = filename.c_str();
        char FullImageName2[100];
        strcpy(FullImageName2, PathName1);
        strcat(FullImageName2, ImageName2);

        IplImage* src1 = cvLoadImage( FullImageName2, 1 );

        IplImage* gray = cvCreateImage( cvGetSize(src1), IPL_DEPTH_8U, 1 );
        cvCvtColor( src1, gray, CV_BGR2GRAY );

        int w = src1->width;
        int h = src1->height;

        int BlackCount = 0;
        int WhiteCount = 0;
        float Ratio = 0.0;
        unsigned short int *image = new unsigned short int [w*h];


 	IplImage* pImg = cvCreateImage(cvGetSize(src1), IPL_DEPTH_8U, 1);

	unsigned short int *Mask = new unsigned short int [w*h];
	for (unsigned short int j=0; j < h ; j++)
	for (unsigned short int i=0; i < w ; i++)
		Mask[j*w+i]=0;


    	float fR, fG, fB;
    	float fV;
    	const float FLOAT_TO_BYTE = 255.0f;
    	const float BYTE_TO_FLOAT = 1.0f / FLOAT_TO_BYTE;
    	//IplImage *imageHSV = cvCreateImage(cvGetSize(src1), 8, 3);

    	//int max = 0;
    	int count = 0;
    	//int min = 255;
    	//int n = 0;
    	int rowSizeRGB = src1->widthStep;	// Size of row in bytes, including extra padding.
    	char *imRGB = src1->imageData;	// Pointer to the start of the image pixels.

        for (unsigned short int y=0; y < h; y++)   // height
    	for (unsigned short int x=0; x < w; x++)    //  width
    		{
    		    CvScalar s;
    		    s = cvGet2D(gray, y, x);
    		    image[y*w+x] = s.val[0];
    		    if (image[y*w+x] <= 213)      ///   for new is 213  for old is 220
    		    	BlackCount++;
    		    if (image[y*w+x] > 213)		///   for new is 213  for old is 220
    		    	WhiteCount++;


    			// Get the RGB pixel components. NOTE that OpenCV stores RGB pixels in B,G,R order.
    			uchar *pRGB = (uchar*)(imRGB + y*rowSizeRGB + x*3);
    			int bB = *(uchar*)(pRGB+0);	// Blue component
    			int bG = *(uchar*)(pRGB+1);	// Green component
    			int bR = *(uchar*)(pRGB+2);	// Red component
    			// Convert from 8-bit integers to floats.
    			fR = bR * BYTE_TO_FLOAT;
    			fG = bG * BYTE_TO_FLOAT;
    			fB = bB * BYTE_TO_FLOAT;
    			// Convert from RGB to HSV, using float ranges 0.0 to 1.0.
    			float  fMax;
    			// Get the min and max, but use integer comparisons for slight speedup.
    			if (bB < bG) {
    				if (bB < bR) {
    					if (bR > bG) {
    						fMax = fR;
    					}
    					else {
    						fMax = fG;
    					}
    				}
    				else {
    					fMax = fG;
    				}
    			}
    			else {
    				if (bG < bR) {
    					if (bB > bR) {
    						fMax = fB;
    					}
    					else {
    						fMax = fR;
    					}
    				}
    				else {
    					fMax = fB;
    				}
    			}
    			fV = fMax;				// Value (Brightness).

    			int bV = (int)(0.5f + fV * 255.0f);

    			if ( bV<=100 )
			{
				Mask[y*w+x]=255;
				count++;
			}

    		}

	    for(int x=0;x<w;x++)
	    for(int y=0;y<h;y++)
			{
				CvScalar s;
				s = cvGet2D(pImg, y,x);
				s.val[0] = Mask[y*w+x];
				cvSet2D (pImg, y, x, s);
			}


	    CvMemStorage * storage = cvCreateMemStorage(0);
	    CvSeq * contour = 0;

	    cvFindContours( pImg, storage, &contour, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));
	    int area,maxArea = 10;//设面积最大值大于10Pixel
	    for(;contour;contour = contour->h_next)
	    {
	    	area = fabs(cvContourArea( contour, CV_WHOLE_SEQ )); //获取当前轮廓面积
	    	if(area > maxArea)
	    		maxArea = area;
	    }


    	Ratio = (float)BlackCount/(float)WhiteCount;

    	if (i%100 == 0)
    		cout <<"i = "<<  i  <<endl;
    	//cout <<"Ratio in "<<  i+1  <<" = "<< Ratio <<endl;

        float Threshold = 5;    						///////////////////////  set the threshold,
        if ((Ratio > Threshold)  &&  (b[i] < 60.0)  &&  (count > 20000) &&  (maxArea < 5000) )
        {
    		int row1 = i/ColNum;
    		int col1 = i%ColNum;
    		// here 120 = w/16, 67.5 = h/16, we can use first multiply then divide to get rid of non-equal!!!!
    		//cvRectangle( src, cvPoint(col1*120, (int)(row1*67.5)), cvPoint(col1*120+120, (int)(row1*67.5+67.5)), CV_RGB( 255,0,0), 3,8,0 );
    		cvRectangle( src, cvPoint(col1*120/step, (int)(row1*67.5/step)), cvPoint(col1*120/step+120, (int)(row1*67.5/step+67.5)), CV_RGB( 255,0,0), 3,8,0 );
        }
        if ((Ratio <= Threshold)  || (b[i] >= 60.0)   ||  (count <= 20000) || (maxArea >= 5000))
        {
        	b[i] = 0.0;
        }



        delete [] image;
        delete [] Mask;

        cvReleaseImage (&src1);
        cvReleaseImage (&gray);
        cvReleaseImage (&pImg);

        cvReleaseMemStorage (&storage);
        //cvClearSeq (contour);
        //cvReleaseSeq (&contour);
        //cvReleaseImage (&contour);

    }


	///////  just for test
    const char *TextName5 = argv[6];
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
    cerr<<"open OnlyRatio.txt error!\n";
    return 0;
    }

    for (int i = 0;i<TotalNumBig;i++)
    {
    	outfile6<<i+1<<"	"<< b[i] <<endl;		///////////////////////////////////////////  should change here
    }


    outfile6.close();
    ///////  text ends here


    const char *ImageName3 = "BoxDAB.jpg";
    char FullImageName3[100];
    strcpy(FullImageName3, PathName1);
    strcat(FullImageName3, ImageName3);

    cvSaveImage(FullImageName3,src);			/////////////////////////////  Just for test

    // poping maximum with index
    for(int i = 0;i<Lengthb-1;i++)
    {
    	float max = 0.0;
    	int indexmax = 0;
    	int maxindexb = 0;

        for(int j = i;j<Lengthb;j++)
        {
        	if( b[j]>max )
        	{
        		max=b[j];
        		indexmax = j;
        		maxindexb = indexb[indexmax];
        	}
        }
        if(i!=indexmax)
        {
        	for(int m=indexmax; m>i; m--)
        	{
            	b[m] = b[m-1];
            	indexb[m] = indexb[m-1];
        	}
        	b[i] = max;
        	indexb[i] = maxindexb;
        }
    }

    for(int i = 0;i<100;i++)
    	cout<<indexb[i]<<"	"<< b[i] <<endl;


    IplImage* src2 = cvLoadImage( FullImageName1, 1 ); ///////      This is the input 2  "DAB"      //////   should be changed here

						///////////////////////////////////////////////////////////////////////
						///////////////////////////////////////////////////////////////////////
    int TopNum = 10;    ///////////////////////////////////////////////////////////////////////  here I set the TopNum to 10
						///////////////////////////////////////////////////////////////////////
						///////////////////////////////////////////////////////////////////////

    for(int i = 0;i<TopNum;i++)												////////////////  Top 10
    {

		int row2 = (indexb[i]-1)/ColNum;
		int col2 = (indexb[i]-1)%ColNum;
		cout<<row2+1<<"	"<<col2+1<<"	"<<b[i]<<endl;
		//cvRectangle( src, cvPoint(col1*120, (int)(row1*67.5)), cvPoint(col1*120+120, (int)(row1*67.5+67.5)), CV_RGB( 255,0,0), 3,8,0 );
	    cvRectangle( src2, cvPoint(col2*120/step, (int)(row2*67.5/step)), cvPoint(col2*120/step+120, (int)(row2*67.5/step+67.5)), CV_RGB( 255,0,0), 3,8,0  );

    }

    const char *ImageName4 = "AnnotatedDAB.jpg";
    char FullImageName4[100];
    strcpy(FullImageName4, PathName1);
    strcat(FullImageName4, ImageName4);

    cvSaveImage(FullImageName4,src2);  ///////      path + name     //////   should be changed here

	///////  just for test
    const char *TextName3 = argv[5];
    char FullTextName3[100];
    strcpy(FullTextName3, PathName1);
    strcat(FullTextName3, TextName3);

    cout << "FullTextName3 = "<< FullTextName3 << endl;

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
    cerr<<"open Top10.txt error!\n";
    return 0;
    }

    for(int i = 0;i<TopNum;i++)												////////////////  Top 10
    {
		int row2 = (indexb[i]-1)/ColNum;
		int col2 = (indexb[i]-1)%ColNum;
		outfile4<<row2+1<<"	"<<col2+1<<"	"<<b[i]<<endl;		///////////////////////////////////////////  should change here
    }

    for(int i = 0;i<10;i++)
    	//cout<<indexb[i]<<"	"<< b[i] <<endl;
	outfile4<<indexb[i]<<"	"<< b[i] <<endl;

    outfile4.close();
    ///////  text ends here

    return 0;

}





