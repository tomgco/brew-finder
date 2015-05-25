#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

Mat src;
Mat image, src_gray;

void cannyThreshold(int, void *) {
  RNG rng(12345);
  int kernel_size = 3;
  Mat detected_edges, dst;
  vector< vector<Point> > contours;
  vector< Vec4i > hierarchy;
  int lowThreshold = 45;
  int const max_lowThreshold = 100;
  int ratio = 3;

  blur(src_gray, detected_edges, Size(4,4));
  Canny(detected_edges, detected_edges, lowThreshold, lowThreshold * ratio, kernel_size);
  /* dilate(detected_edges, detected_edges, Mat(), Point(-1,-1)); */
  dst = Scalar::all(0);
  src.copyTo(dst, detected_edges);

  findContours(detected_edges, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

  vector< vector<Point> > contours_poly( contours.size() );
  vector<Rect> boundRect( contours.size() );
  vector<Point2f>center( contours.size() );
  vector<float>radius( contours.size() );

  // create bounding boxes of contours
  for(int i = 0; i < contours.size(); i++) {
    approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
    boundRect[i] = boundingRect(Mat(contours_poly[i]));
  }

  // Draw bounding boxes around contours.
  Mat drawing = Mat::zeros(detected_edges.size(), CV_8UC3);
  for(int i = 0; i< contours.size(); i++) {
    Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
    drawContours( drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
    rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
  }

  /// Draw contours
  /* Mat drawing = Mat::zeros(detected_edges.size(), CV_8UC3); */

  /* for(int i = 0; i< contours.size(); i++) { */
  /*   Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) ); */
  /*   drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point()); */
  /* } */

  imshow("Display Image", dst);
  imshow("Display Image2", drawing);
}

int main(int argc, char** argv ) {

  if ( argc != 2 ) {
    printf("usage: beerdetect.out <Image_Path>\n");
    return -1;
  }

  image = imread( argv[1], 1 );

  if (!image.data) {
    printf("No image data \n");
    return -1;
  }

  resize(image, src, cvSize(0, 0), 0.6, 0.6);

  GaussianBlur(src, src, Size(3, 3), 0, 0, BORDER_DEFAULT);
  cvtColor(src, src_gray, CV_RGB2GRAY);
  namedWindow("Display Image", WINDOW_AUTOSIZE);
  /* createTrackbar( " Threshold:", "Display Image", &lowThreshold, max_lowThreshold, cannyThreshold ); */
  cannyThreshold(40, 0);

  waitKey(0);

  return 0;
}
