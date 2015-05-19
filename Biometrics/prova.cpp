//
//  prova.cpp
//  Biometrics
//
//  Created by Marco Spizzico on 10/02/15.
//
//

#include "prova.h"
/**
 * @file objectDetection2.cpp
 * @author A. Huaman ( based in the classic facedetect.cpp in samples/c )
 * @brief A simplified version of facedetect.cpp, show how to load a cascade classifier and how to find objects (Face + eyes) in a video stream - Using LBP here
 */
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

/** Function Headers */
void detectAndDisplay( Mat frame );

/** Global variables */
String face_cascade_name = "/usr/local/share/OpenCV/haarcascades/haarcascade_frontalface_alt_tree.xml";
String eyes_cascade_name = "/usr/local/share/OpenCV/haarcascades/haarcascade_eye_tree_eyeglasses.xml";
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
string window_name = "Capture - Face detection";
string capture_window = "Grayscale Snapshot Taken";
string histogram_window = "Image Histogram";
Mat cropped_image;

RNG rng(12345);

void on_push(int state, void*) {}

void show_histogram(std::string const& name, cv::Mat1b const& image)
{
    // Set histogram bins count
    int bins = 256;
    int histSize[] = {bins};
    // Set ranges for histogram bins
    float lranges[] = {0, 256};
    const float* ranges[] = {lranges};
    // create matrix for histogram
    cv::Mat hist;
    int channels[] = {0};
    
    // create matrix for histogram visualization
    int const hist_height = 256;
    cv::Mat3b hist_image = cv::Mat3b::zeros(hist_height, bins);
    
    cv::calcHist(&image, 1, channels, cv::Mat(), hist, 1, histSize, ranges, true, false);
    
    double max_val=0;
    minMaxLoc(hist, 0, &max_val);
    
    // visualize each bin
    for(int b = 0; b < bins; b++) {
        float const binVal = hist.at<float>(b);
        int   const height = cvRound(binVal*hist_height/max_val);
        cv::line
        ( hist_image
         , cv::Point(b, hist_height-height), cv::Point(b, hist_height)
         , cv::Scalar::all(255)
         );
    }
    cv::imshow(name, hist_image);
}


/**
 * @function main
 */
int main( void )
{
  VideoCapture capture;
  Mat frame;
  
   //-- 1. Load the cascade
  if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };
  if( !eyes_cascade.load( eyes_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };

  //-- 2. Read the video stream
  capture.open( -1 );
    capture.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
  if( capture.isOpened() )
  {
    for(;;)
    {
      capture >> frame;

      //-- 3. Apply the classifier to the frame
      if( !frame.empty() )
      {
          
          //imshow(window_name, frame);
          detectAndDisplay( frame );
      }
      else
       { printf(" --(!) No captured frame -- Break!"); break; }

      int c = waitKey(10);
      if( (char)c == 'q' ) { break; }
      if( (char)c == ' ') {
          imwrite(".*/Desktop/capture.jpg",frame);
          Mat frame_gray;
          cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
          equalizeHist( frame_gray, frame_gray );
          imshow(capture_window, frame_gray);
          //imwrite("/Users/Ulysses_D/Desktop/capture_gray.jpg",frame_gray);
          
          
          
      }
      if ( (char) c == 'h'){
          imwrite(".*/Desktop/capture.jpg",frame);
          Mat frame_gray;
          cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
          equalizeHist( frame_gray, frame_gray );
          imshow(capture_window, cropped_image);
          show_histogram(histogram_window, cropped_image);
        
      }
    }
  }
  return 0;
}

/**
 * @function detectAndDisplay
 */
void detectAndDisplay( Mat frame )
{
   std::vector<Rect> faces;
   Mat frame_gray;

   cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
   GaussianBlur(frame_gray, frame_gray, Size(7,7), 5, 3, BORDER_CONSTANT);
   equalizeHist( frame_gray, frame_gray );
   
   //-- Detect faces
   face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );

   for( size_t i = 0; i < faces.size(); i++ )
    {
      Mat faceROI = frame_gray( faces[i] );
      //std::vector<Rect> eyes;

      //-- In each face, detect eyes
      //eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );
      //if( eyes.size() == 2)
      //{
         //-- Draw the face
         //Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
         //ellipse( frame, center, Size( faces[i].width/2, faces[i].height/2), 0, 0, 360, Scalar( 0, 0, 255 ), 2, 8, 0 );
        
        cropped_image = faceROI.clone();
        rectangle(frame, Point(faces[i].x, faces[i].y), Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height), Scalar( 0, 255, 0 ));
         //for( size_t j = 0; j < eyes.size(); j++ )
         //{ //-- Draw the eyes
            //Point eye_center( faces[i].x + eyes[j].x + eyes[j].width/2, faces[i].y + eyes[j].y + eyes[j].height/2 );
            //int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
            //circle( frame, eye_center, radius, Scalar( 0, 255, 0 ), 3, 8, 0 );
            //circle(frame, eye_center, 1, Scalar( 0, 255, 0 ), -1, 8);
          //}
       //}
    }
   //-- Show what you got
   imshow( window_name, frame);
}
