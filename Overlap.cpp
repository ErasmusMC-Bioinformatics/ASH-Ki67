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



//IplImage *OutputImage1;



int main(int argc, char *argv[])
{
	//cout << "argv[1] = "<< argv[1] << endl;
	//cout << "argv[2] = "<< argv[2] << endl;

	const char *PathName1 = argv[1];	/////////////////////////////   Input path name    should be changed here
	//const char *PathName1 = "/home/hao/Ki67Analysis/DAB0/";	/////////////////////////////   Input path name    should be changed here
    const char *ImageName1 = argv[2];
    //const char *ImageName1 = "DAB0";
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

    int TotalNumSmall = width*height;

    cout << "width = "<< width << endl;
    cout << "height = "<< height << endl;
    cout << "TotalNumSmall = "<< TotalNumSmall << endl;





    //int step = 2;    				//   step should input here

    int step = atoi(argv[3]);
    cout << "step = "<< step << endl;

    int Row = (height-2)*step+1;
    int Col = (width-2)*step+1;
    int TotalNumBig = Row*Col;

    cout << "Row = "<< Row << endl;
    cout << "Col = "<< Col << endl;
    cout << "TotalNumBig = "<< TotalNumBig << endl;

    //cout << "25/9 = "<< 25/9 << endl;
    //cout << "25%9 = "<< 25%9 << endl;


    cvReleaseImage(&src);


    char TestImageName[100];
    const char *Postfix = "_x40_z0_result";
    //strcpy(TestImageName, ImageName1);
    strcpy(TestImageName, ImageName1);
    //strcat(TestImageName, ImageName1);


    strcat(TestImageName, Postfix);
    cout << "TestImageName = "<< TestImageName << endl;

	IplImage *OutputImage1 = cvCreateImage(cvSize(ImageW*2,ImageH), IPL_DEPTH_8U, 3);
	IplImage *OutputImage2 = cvCreateImage(cvSize(ImageW,ImageH*2), IPL_DEPTH_8U, 3);
	IplImage *OutputImage3 = cvCreateImage(cvSize(ImageW*2,ImageH*2), IPL_DEPTH_8U, 3);

    for (int j=1;j<Row+1;j++)
    //for (int j=1;j<5;j++)
    {
    	for (int i=1;i<Col+1;i++)
    	//for (int i=1;i<64;i++)
        {
    		/////   non-overlap
        	if ( ((i-1)%step == 0)  &&  ((j-1)%step == 0) )
        	{
				///////  when step = 4, i only = 1,5,9.... , so we have no need to use the ii=i-1,
        		///////  so as the case in 'overlap only in row', we have no need to use the jj=j-1.
            	stringstream input1;
            	input1<<TestImageName<<(i/step+1)+(j/step)*width<<"_i1j1.jpg";	//should use the input DAB here   // here involve j
                string inputname1 = input1.str();

                const char *InputImageName1 = inputname1.c_str();
                char FullInputImageName1 [100];
                strcpy(FullInputImageName1, PathName1);
                strcat(FullInputImageName1, InputImageName1);
                cout << "FullInputImageName1= "<< FullInputImageName1 << endl;

                IplImage* InputImage1 = cvLoadImage( FullInputImageName1, 1 );

            	stringstream output1;
            	output1<<i+(j-1)*Col<<".jpg";				////////   should use the input DAB here  // here involve j
                string outputname1 = output1.str();

                const char *OutputImageName1 = outputname1.c_str();
                char FullOutputImageName1[100];
                strcpy(FullOutputImageName1, PathName1);
                strcat(FullOutputImageName1, OutputImageName1);

                cvSaveImage(FullOutputImageName1,InputImage1);


                cvReleaseImage(&InputImage1);						/////////////////


        	}

        	cout << "i= "<< i << endl;
        	cout << "j= "<< j << endl;






        	////	overlap only in row
        	if ( (i-1)%step != 0  &&  ((j-1)%step == 0) )
        	{
        		int ii = i-1;

            	stringstream input1;
            	input1<<TestImageName<<(ii/step+1)+(j/step)*width<<"_i1j1.jpg";	////////   should use the input DAB here	input1 is +1 // here involve j
                string inputname1 = input1.str();

                const char *InputImageName1 = inputname1.c_str();
                char FullInputImageName1 [100];
                strcpy(FullInputImageName1, PathName1);
                strcat(FullInputImageName1, InputImageName1);
                cout << "FullInputImageName1= "<< FullInputImageName1 << endl;

                IplImage* InputImage1 = cvLoadImage( FullInputImageName1, 1 );


            	stringstream input2;
            	input2<<TestImageName<<(ii/step+2)+(j/step)*width<<"_i1j1.jpg";	////////  should use the input DAB here   input2 is +2  // here involve j
                string inputname2 = input2.str();

                const char *InputImageName2 = inputname2.c_str();
                char FullInputImageName2[100];
                strcpy(FullInputImageName2, PathName1);
                strcat(FullInputImageName2, InputImageName2);
                cout << "FullInputImageName2= "<< FullInputImageName2 << endl;

                IplImage* InputImage2 = cvLoadImage( FullInputImageName2, 1 );

                IplImage* mosaic = cvCreateImage( cvSize(InputImage1->width+InputImage2->width,MAX(InputImage1->height,InputImage2->height)),IPL_DEPTH_8U,3);

                CvRect rect1=cvRect(0,0,InputImage1->width,InputImage1->height);
                cvSetImageROI(mosaic,rect1);
                cvCopy(InputImage1,mosaic);

                CvRect rect2=cvRect(InputImage1->width,0,InputImage2->width,InputImage2->height);
                cvSetImageROI(mosaic,rect2);
                cvCopy(InputImage2,mosaic);

                cvResetImageROI(mosaic);

                CvRect outputrect = cvRect((ii%step)*(InputImage1->width)/step,0,InputImage1->width,InputImage1->height);    //// here not need involve j

                //IplImage *OutputImage1 = cvCreateImage(cvSize(mosaic->width,mosaic->height), IPL_DEPTH_8U, 3);
                OutputImage1 = mosaic;
                cvSetImageROI(OutputImage1, outputrect);

            	stringstream output1;
            	output1<<i+(j-1)*Col<<".jpg";				////////   should use the input DAB here     // here involve j
                string outputname1 = output1.str();

                const char *OutputImageName1 = outputname1.c_str();
                char FullOutputImageName1[100];
                strcpy(FullOutputImageName1, PathName1);
                strcat(FullOutputImageName1, OutputImageName1);

                cvSaveImage(FullOutputImageName1,OutputImage1);


                cvReleaseImage(&InputImage1);
                cvReleaseImage(&InputImage2);
                cvReleaseImage(&mosaic);

                //cvWaitKey(1000);
                //cvReleaseImage(&OutputImage1);

        	}
        	//cvWaitKey(1000);
        	//cvReleaseImage(&OutputImage1);








        	////	overlap only in Column
        	if ( (i-1)%step == 0  &&  ((j-1)%step != 0) )
        	{

          		int ii = i-1;
          		int jj = j-1;


          		stringstream input1;
                input1<<TestImageName<<(ii/step+1)+(jj/step)*width<<"_i1j1.jpg";	////////   should use the input DAB here  input1 is ii/step+1, jj/step
                string inputname1 = input1.str();

                const char *InputImageName1 = inputname1.c_str();
                char FullInputImageName1 [100];
                strcpy(FullInputImageName1, PathName1);
                strcat(FullInputImageName1, InputImageName1);
                cout << "FullInputImageName1= "<< FullInputImageName1 << endl;

                IplImage* InputImage1 = cvLoadImage( FullInputImageName1, 1 );

            	stringstream input2;
                input2<<TestImageName<<(ii/step+1)+(jj/step+1)*width<<"_i1j1.jpg";	////////   should use the input DAB here input2 is ii/step+1, jj/step+1
                string inputname2 = input2.str();

                const char *InputImageName2 = inputname2.c_str();
                char FullInputImageName2[100];
                strcpy(FullInputImageName2, PathName1);
                strcat(FullInputImageName2, InputImageName2);
                cout << "FullInputImageName2= "<< FullInputImageName2 << endl;

                IplImage* InputImage2 = cvLoadImage( FullInputImageName2, 1 );


                IplImage* mosaic = cvCreateImage( cvSize(  MAX(InputImage1->width,InputImage2->width),  InputImage1->height+InputImage2->height ),IPL_DEPTH_8U,3);

                CvRect rect1=cvRect(0,0,InputImage1->width,InputImage1->height);
                cvSetImageROI(mosaic,rect1);
                cvCopy(InputImage1,mosaic);

                CvRect rect2=cvRect(0,InputImage1->height,InputImage2->width,InputImage2->height);
                cvSetImageROI(mosaic,rect2);
                cvCopy(InputImage2,mosaic);

                cvResetImageROI(mosaic);

                CvRect outputrect = cvRect(0,(jj%step)*(InputImage1->height)/step,InputImage1->width,InputImage1->height);    //// here not need involve j

                //IplImage *OutputImage2 = cvCreateImage(cvSize(mosaic->width,mosaic->height), IPL_DEPTH_8U, 3);
                OutputImage2 = mosaic;
                cvSetImageROI(OutputImage2, outputrect);

            	stringstream output1;
            	output1<<i+(j-1)*Col<<".jpg";				////////   should use the input DAB here     // here involve j
                string outputname1 = output1.str();

                const char *OutputImageName1 = outputname1.c_str();
                char FullOutputImageName1[100];
                strcpy(FullOutputImageName1, PathName1);
                strcat(FullOutputImageName1, OutputImageName1);

                //cvSaveImage(FullOutputImageName1,mosaic);		///////  show mosaic
                cvSaveImage(FullOutputImageName1,OutputImage2);


                cvReleaseImage(&InputImage1);
                cvReleaseImage(&InputImage2);
                cvReleaseImage(&mosaic);
                //cvReleaseImage(&OutputImage1);




        	}

        	////	overlap both in Row and Column
        	if ( (i-1)%step != 0  &&  ((j-1)%step != 0) )
        	{
          		int ii = i-1;
          		int jj = j-1;

          		stringstream input1;
                input1<<TestImageName<<(ii/step+1)+(jj/step)*width<<"_i1j1.jpg";	////////   should use the input DAB here
                string inputname1 = input1.str();

                const char *InputImageName1 = inputname1.c_str();
                char FullInputImageName1 [100];
                strcpy(FullInputImageName1, PathName1);
                strcat(FullInputImageName1, InputImageName1);
                cout << "FullInputImageName1= "<< FullInputImageName1 << endl;
                IplImage* InputImage1 = cvLoadImage( FullInputImageName1, 1 );


            	stringstream input2;
                input2<<TestImageName<<(ii/step+2)+(jj/step)*width<<"_i1j1.jpg";	////////   should use the input DAB here
                string inputname2 = input2.str();

                const char *InputImageName2 = inputname2.c_str();
                char FullInputImageName2[100];
                strcpy(FullInputImageName2, PathName1);
                strcat(FullInputImageName2, InputImageName2);
                cout << "FullInputImageName2= "<< FullInputImageName2 << endl;
                IplImage* InputImage2 = cvLoadImage( FullInputImageName2, 1 );


          		stringstream input3;
                input3<<TestImageName<<(ii/step+1)+(jj/step+1)*width<<"_i1j1.jpg";	////////   should use the input DAB here
                string inputname3 = input3.str();

                const char *InputImageName3 = inputname3.c_str();
                char FullInputImageName3 [100];
                strcpy(FullInputImageName3, PathName1);
                strcat(FullInputImageName3, InputImageName3);
                cout << "FullInputImageName3= "<< FullInputImageName3 << endl;
                IplImage* InputImage3 = cvLoadImage( FullInputImageName3, 1 );


          		stringstream input4;
                input4<<TestImageName<<(ii/step+2)+(jj/step+1)*width<<"_i1j1.jpg";	////////   should use the input DAB here
                string inputname4 = input4.str();

                const char *InputImageName4 = inputname4.c_str();
                char FullInputImageName4 [100];
                strcpy(FullInputImageName4, PathName1);
                strcat(FullInputImageName4, InputImageName4);
                cout << "FullInputImageName4= "<< FullInputImageName4 << endl;
                IplImage* InputImage4 = cvLoadImage( FullInputImageName4, 1 );

                IplImage* mosaic = cvCreateImage( cvSize(  InputImage1->width+InputImage2->width,  InputImage1->height+InputImage2->height ),IPL_DEPTH_8U,3);

                CvRect rect1=cvRect(0,0,InputImage1->width,InputImage1->height);
                cvSetImageROI(mosaic,rect1);
                cvCopy(InputImage1,mosaic);

                CvRect rect2=cvRect(InputImage1->width,0,InputImage2->width,InputImage2->height);
                cvSetImageROI(mosaic,rect2);
                cvCopy(InputImage2,mosaic);

                CvRect rect3=cvRect(0,InputImage1->height,InputImage2->width,InputImage2->height);
                cvSetImageROI(mosaic,rect3);
                cvCopy(InputImage3,mosaic);

                CvRect rect4=cvRect(InputImage1->width,InputImage1->height,InputImage2->width,InputImage2->height);
                cvSetImageROI(mosaic,rect4);
                cvCopy(InputImage4,mosaic);

                cvResetImageROI(mosaic);

                CvRect outputrect = cvRect((ii%step)*(InputImage1->width)/step,(jj%step)*(InputImage1->height)/step,InputImage1->width,InputImage1->height);    //// here not need involve j

                //IplImage *OutputImage3 = cvCreateImage(cvSize(mosaic->width,mosaic->height), IPL_DEPTH_8U, 3);
                OutputImage3 = mosaic;
                cvSetImageROI(OutputImage3, outputrect);


            	stringstream output1;
            	output1<<i+(j-1)*Col<<".jpg";				////////   should use the input DAB here     // here involve j
                string outputname1 = output1.str();

                const char *OutputImageName1 = outputname1.c_str();
                char FullOutputImageName1[100];
                strcpy(FullOutputImageName1, PathName1);
                strcat(FullOutputImageName1, OutputImageName1);

                //cvSaveImage(FullOutputImageName1,mosaic);		///////  show mosaic
                cvSaveImage(FullOutputImageName1,OutputImage3);


                cvReleaseImage(&InputImage1);
                cvReleaseImage(&InputImage2);
                cvReleaseImage(&InputImage3);
                cvReleaseImage(&InputImage4);
                cvReleaseImage(&mosaic);
                //cvReleaseImage(&OutputImage1);
        	}



        }
    }










    return 0;

}







