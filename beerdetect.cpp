#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

Mat src;
Mat image, src_gray;
Mat ero_dst;

int erosion_elem = 1;
int erosion_size = 1;
int dilation_elem = 0;
int dilation_size = 0;
int blag = 0;
int const max_kernel_size = 21;
Mat detected_edges, dst;

void cannyThreshold(int, void *) {
  RNG rng(12345);
  int kernel_size = 3;
  vector< vector<Point> > contours;
  vector< Vec4i > hierarchy;
  int lowThreshold = 50;
  int ratio = 5;

  Canny(detected_edges, detected_edges, lowThreshold, lowThreshold * ratio, kernel_size);
  dst = Scalar::all(0);
  src.copyTo(dst, detected_edges);

  findContours(detected_edges, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

  vector< vector<Point> > contours_poly( contours.size() );
  vector<Rect> boundRect( contours.size() );
  vector<Point2f>center( contours.size() );
  vector<float>radius( contours.size() );


  int largestBox = 0;
  int largestIndex = 0;
  // create bounding boxes of contours
  for(int i = 0; i < contours.size(); i++) {
    approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
    int a = contourArea(contours_poly[i], false);
    if (a > largestBox) {
      largestBox = a;
      largestIndex = i;
    }
    boundRect[i] = boundingRect(Mat(contours_poly[i]));
  }

  // Draw bounding boxes around contours.
  Mat drawing = Mat::zeros(detected_edges.size(), CV_8UC3);
  for(int i = 0; i< contours.size(); i++) {
    Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
    drawContours( drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
    rectangle( drawing, boundRect[largestIndex].tl(), boundRect[largestIndex].br(), color, 2, 8, 0 );
  }

  imshow("Display Image", dst);
  imshow("Display Image2", drawing);
}

void normaliseImage() {
  GaussianBlur(src_gray, detected_edges, Size(1, 1), 2, 1, BORDER_DEFAULT);
  /* blur(src_gray, detected_edges, Size(4,4)); */
  /* dilate(detected_edges, detected_edges, Mat(), Point(-1,-1)); */
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

  cvtColor(src, src_gray, CV_RGB2GRAY);
  namedWindow("Display Image", WINDOW_AUTOSIZE);
  /* createTrackbar( " Threshold:", "Display Image", &lowThreshold, max_lowThreshold, cannyThreshold ); */

  /// Default start
  normaliseImage();
  cannyThreshold(40, 0);

  waitKey(0);

  return 0;
}
