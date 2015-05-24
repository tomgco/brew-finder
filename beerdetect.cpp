#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv ) {
  Mat image, src_gray;
  Mat grad;
  int scale = 1;
  int delta = 0;
  int ddepth = CV_16S;

  if ( argc != 2 ) {
    printf("usage: beerdetect.out <Image_Path>\n");
    return -1;
  }

  image = imread( argv[1], 1 );

  if (!image.data) {
    printf("No image data \n");
    return -1;
  }
  Mat src;
  resize(image, src, cvSize(0, 0), 0.5, 0.5);

  GaussianBlur(src, src, Size(3,3), 0, 0, BORDER_DEFAULT);
  cvtColor(src, src_gray, CV_RGB2GRAY);

  Mat grad_x, grad_y;
  Mat abs_grad_x, abs_grad_y;

  /// Gradient X
  //Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
  Sobel( src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
  convertScaleAbs( grad_x, abs_grad_x );

  /// Gradient Y
  //Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
  Sobel( src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
  convertScaleAbs( grad_y, abs_grad_y );

  /// Total Gradient (approximate)
  addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );

  namedWindow("Display Image", WINDOW_AUTOSIZE );
  imshow("Display Image", grad);

  waitKey(0);

  return 0;
}
