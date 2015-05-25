#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

Mat src;
Mat image, src_gray;
Mat detected_edges, dst;
vector< vector<Point> > contours;
vector< Vec4i > hierarchy;
int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;
RNG rng(12345);

void cannyThreshold(int, void *) {
  blur(src_gray, detected_edges, Size(4,4));
  Canny(detected_edges, detected_edges, lowThreshold, lowThreshold * ratio, kernel_size);
  dst = Scalar::all(0);
  src.copyTo(dst, detected_edges);

  findContours(detected_edges, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

  /// Draw contours
  Mat drawing = Mat::zeros(detected_edges.size(), CV_8UC3);

  for(int i = 0; i< contours.size(); i++) {
    Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
    drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
  }

/*   vector<Vec4i> lines; */
/*   Mat linedst; */
/*   cvtColor(detected_edges, linedst, CV_GRAY2BGR); */

/*   HoughLinesP(detected_edges, lines, 1, CV_PI/180, 50, 50, 10 ); */
/*   for( size_t i = 0; i < lines.size(); i++ ) { */
/*     Vec4i l = lines[i]; */
/*     line(linedst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA); */
/*   } */

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
  lowThreshold = 50;
  cannyThreshold(40, 0);

  waitKey(0);

  return 0;
}
