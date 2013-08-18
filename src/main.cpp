//
//  single.cpp
//  ImpWshed
//
//  Created by Di Yang on 26/03/12.
//  Copyright (c) 2012 The Australian National University. All rights reserved.
//

#include "VisWatershed.h"

IplImage* marker_mask = 0, *output = 0;
IplImage* markers = 0;
IplImage* img0 = 0, *img = 0, *img2=0, *img_gray = 0, *wshed = 0;
IplImage* res_img = 0, * res_seg = 0;
CvPoint prev_pt = {-1,-1};
using namespace std;

void on_mouse( int event, int x, int y, int flags, void* param )
{
  if( !img )
    return;
	
  if( event == CV_EVENT_LBUTTONUP || !(flags&CV_EVENT_FLAG_LBUTTON) )
    prev_pt = cvPoint(-1,-1);
  else if( event == CV_EVENT_LBUTTONDOWN )
    prev_pt = cvPoint(x,y);
  else if( event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON) )
  {
    CvPoint pt = cvPoint(x,y);
    if( prev_pt.x < 0 )
      prev_pt = pt;
    cvLine( marker_mask, prev_pt, pt, cvScalarAll(255), 3, 8, 0 );
    cvLine( img, prev_pt, pt, cvScalar(255,0,0), 3, 8, 0 );
    prev_pt = pt;
    cvShowImage( "image", img );
  }
}
void bind_line()
{
  cout<<"     \033[33m*-------------------------------------------------------------------*\033[m"<<endl;
}
void info_key()
{
  bind_line();
  cout<<"     \033[33m| Hot keys:                                                         |\033[m"<<endl;
  cout<<"     \033[33m|\033[m \033[31;4;1mPress 'w' or 'ENTER' - run watershed with viscous force algorithm\033[m \033[33m|\033[m"<<endl;
  cout<<"     \033[33m|\033[m \033[31;4;1mPress 'ESC' - exit the program\033[m                                    \033[33m|\033[m"<<endl;
  bind_line();
}

int main( int argc, char** argv )
{
    CvRNG rng = cvRNG(-1);
    CvSize size;
    int comp_count=0;
    bool IS_MASK = false;
    
    if(argc < 2){
      cout<<"     \033[31;1m WARNING:\033[m\033[33;5m Please locate the input image\033[m"<<endl;
      return 0;
    }
    
    if(!(img0 = cvLoadImage(argv[1],1))){
      cout<<"     \033[31;1m WARNING:\033[m\033[33;5m No Input Image Found\033[m"<<endl;
      return 0;
    } 
    
    size = cvGetSize(img0);
    
    img = cvCloneImage( img0 );
    img_gray = cvCloneImage( img0 );
    wshed = cvCloneImage( img0 );
    marker_mask = cvCreateImage( size, IPL_DEPTH_8U, 1 );
    markers = cvCreateImage( size, IPL_DEPTH_32S, 1 ); 
    cvZero( wshed );

    cvNamedWindow( "image", 1 );
    cvNamedWindow( "segmentation", 1 );
    cvMoveWindow( "image", 0,0 );
    cvMoveWindow( "segmentation", size.width*1.5,0 );
    
    cvShowImage( "image", img );
    cvShowImage( "segmentation", wshed );
    if(argc == 3){      
      if((marker_mask = cvLoadImage(argv[2],0))){
	cvConvertScale(marker_mask, markers, 1, 0);
	IS_MASK =true;
	system("clear"); 
	info_key();
      }
    }
    if (!IS_MASK){
       cvCvtColor( img, marker_mask, CV_RGB2GRAY );
       cvCvtColor( marker_mask, img_gray, CV_GRAY2RGB );
       cvZero( marker_mask );
       system("clear");
       info_key();       
       cout<<"     \033[33m|\033[m \033[31;1m WARNING:\033[m\033[33;5m No Mask Image Found\033[m                                     \033[33m|\033[m"<<endl;
       cout<<"     \033[33m|\033[m \033[32;m (before running it, roughly mark the areas on the image)\033[m         \033[33m|\033[m"<<endl;
       cout<<"     \033[33m|\033[m \033[31;4;1mPress 'r' - resort the original image\033[m                             \033[33m|\033[m"<<endl;
       bind_line();
       cvSetMouseCallback( "image", on_mouse, 0 );
    }
   	
    while(true)
    {
      char c = cvWaitKey(0);
      if( c == 27 ) break;
      if( c == 'r' ){
	cvZero( marker_mask );
	cvZero( wshed );
	cvCopy( img0, img );
	cvShowImage( "image", img );
	cvShowImage( "segmentation", wshed);
      }
		
      if( c == 'w' || c == '\n' )
      {
	 CvMemStorage* storage = cvCreateMemStorage(0);
	 CvSeq* contours = 0;
	 CvMat* color_tab;
        
	 if (!IS_MASK){
	   cvFindContours(marker_mask, storage, &contours, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	  cvZero( markers );
	  for( ; contours != 0; contours = contours->h_next, comp_count++ )
	    cvDrawContours( markers, contours, cvScalarAll(comp_count+1),cvScalarAll(comp_count+1), -1, -1, 8, cvPoint(0,0) );
	 }
	 
         double t = (double)cvGetTickCount();
	 VisWatershed( img0, markers, 2, 0.02);
	 t = (double)cvGetTickCount() - t;
	 cout<< "exec time for viscous watershed = " << t/(cvGetTickFrequency()*1.0e3)<<"ms"<<endl;
			
	 // paint the watershed image
	 for( int i = 0; i < markers->height; i++ )
	   for( int j = 0; j < markers->width; j++ ){
	     int idx = CV_IMAGE_ELEM( markers, int, i, j );
	     uchar* dst = &CV_IMAGE_ELEM( wshed, uchar, i, j*3 );
					
	     if( idx == -1 ){
	       dst[0] = (uchar)0; dst[1] = (uchar)255; dst[2] = (uchar)0;
	       if (i+1<=markers->height && i-1>=0 && j+1<=markers->width && j-1>=0) {
		 for (int n=-1; n<=1; n++)
		   for (int m=-1; m<=1; m++){
		     uchar* dst = &CV_IMAGE_ELEM( wshed, uchar, i+n, (j+m)*3 );
		     dst[0] = (uchar)0; dst[1] = (uchar)0; dst[2] = (uchar)255;
                   }
	       }
             }
	   }
	 cvAddWeighted( wshed, 0.5, img_gray, 0.5, 0, wshed);
         cvShowImage( "segmentation", wshed );
         
	 cvConvertScale(markers, marker_mask, 1, 0);
	 
	 string str = argv[1];
	 string _str0 = "_seg";
	 string _str1 = "_res";
	 string _str2 = "_ini";
	 string _ext = ".png";
	 
	 _str0.insert(0, str, 0, str.length()-4);
	 _str0.insert(_str0.length(), _ext);
	 
	 _str1.insert(0, str, 0, str.length()-4);
	 _str1.insert(_str1.length(), _ext);
          
          

	 if (!IS_MASK){
	   _str2.insert(0, str, 0, str.length()-4);
	   _str2.insert(_str2.length(), _ext);
	   cvSaveImage( (char*)_str2.c_str(), img);
         }
	 cvSaveImage((char*)_str0.c_str(), wshed);
	 cvSaveImage((char*)_str1.c_str(), marker_mask);
      }
    }
    cvDestroyWindow("image");
    cvDestroyWindow("segmentation");
    //cvReleaseImage(& marker_mask);
    //cvReleaseImage(& markers);
    //cvReleaseImage(& wshed);
    //cvReleaseImage(& img);
    //cvReleaseImage(& img0);
    return 1;
}
