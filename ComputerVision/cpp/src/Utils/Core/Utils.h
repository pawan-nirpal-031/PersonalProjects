#ifndef UTILS
#define UTILS

#include "Core.h"

static void displayImage(Mat &img, unsigned int time = 0,
                         string title = "frame") {
  imshow(title, img);
  waitKey(time);
}

static bool isValidPoint(Mat &img, int x, int y) {
  int rows = img.rows;
  int cols = img.cols;
  return (x >= 0 and x < rows and y >= 0 and y < cols);
}

static void performDiff(Mat img1, Mat img2) {
  int rows = img1.rows;
  int cols = img2.cols;
  Mat diff(rows, cols, CV_8U);
  bool zero = true;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      int d = (static_cast<int>(img1.at<u_char>(i, j)) -
               static_cast<int>(img2.at<u_char>(i, j)));
      diff.at<u_char>(i, j) = d;
      if (d != 0)
        zero = false;
    }
  }
  displayImage(diff, 0, "diff image");
}

static int getValIntOf(Mat &img, int x, int y) {
  return static_cast<int>(img.at<u_char>(x, y));
}

Mat getGrayScaleImage(Mat &img) {
  int rows = img.rows;
  int cols = img.cols;
  Mat Gray(rows, cols, CV_8U);
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      Vec3b intensity = img.at<Vec3b>(i, j);
      int blue = intensity[0];
      int green = intensity[1];
      int red = intensity[2];
      double tval = 0.299 * (red) + 0.587 * (green) + 0.114 * (blue);
      Gray.at<u_char>(i, j) = static_cast<u_char>(min(255.0, tval));
    }
  }
  return Gray;
}

#endif