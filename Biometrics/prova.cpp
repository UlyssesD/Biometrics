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
String face_cascade_name = "/usr/local/share/OpenCV/lbpcascades/lbpcascade_frontalface.xml";
String eyes_cascade_name = "/usr/local/share/OpenCV/haarcascades/haarcascade_eye_tree_eyeglasses.xml";
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
string window_name = "Capture - Face detection";
string capture_window = "Grayscale Snapshot Taken";

RNG rng(12345);

void on_push(int state, void*) {}
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
          
          imshow(window_name, frame);
          //detectAndDisplay( frame );
      }
      else
       { printf(" --(!) No captured frame -- Break!"); break; }

      int c = waitKey(10);
      if( (char)c == 'q' ) { break; }
      if( (char)c == ' ') {
          //imwrite("/Users/Ulysses_D/Desktop/capture.jpg",frame);
          Mat frame_gray;
          cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
          equalizeHist( frame_gray, frame_gray );
          imshow(capture_window, frame_gray);
          //imwrite("/Users/Ulysses_D/Desktop/capture_gray.jpg",frame_gray);
          
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
   equalizeHist( frame_gray, frame_gray );

   //-- Detect faces
   face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0, Size(80, 80) );

   for( size_t i = 0; i < faces.size(); i++ )
    {
      Mat faceROI = frame_gray( faces[i] );
      std::vector<Rect> eyes;

      //-- In each face, detect eyes
      eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );
      if( eyes.size() == 2)
      {
         //-- Draw the face
         Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
         ellipse( frame, center, Size( faces[i].width/2, faces[i].height/2), 0, 0, 360, Scalar( 0, 0, 255 ), 2, 8, 0 );

         for( size_t j = 0; j < eyes.size(); j++ )
          { //-- Draw the eyes
            Point eye_center( faces[i].x + eyes[j].x + eyes[j].width/2, faces[i].y + eyes[j].y + eyes[j].height/2 );
            int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
            circle( frame, eye_center, radius, Scalar( 0, 255, 0 ), 3, 8, 0 );
          }
       }

    }
   //-- Show what you got
   imshow( window_name, frame );
}
