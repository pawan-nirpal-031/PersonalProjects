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

static int getIntValOf(Mat &img, int x, int y) {
  return static_cast<int>(img.at<u_char>(x, y));
}

static double getDoubleValOf(Mat &img, int x, int y) {
  return static_cast<double>(img.at<u_char>(x, y));
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
//  oldMin,  oldMax,  newMin,  newMax,  oldVal
double getNormalizedValue(double oldMin, double oldMax, double newMin,
                          double newMax, double oldVal) {
  double oldRange = (oldMax - oldMin);
  double newRange = (newMax - newMin);
  double newVal = (newMin + ((newRange * (oldVal - oldMin)) / (oldRange)));
  return newVal;
}

bool isImgPoint(int rows, int cols, int x, int y) {
  return (x >= 0 and x < rows and y >= 0 and y < cols);
}

static float square(float x) { return x * x; }

static float computeGaussianFunc(float x, float y, float mu, float sigma) {
  float val =
      exp(-0.5 * ((square((x - mu) / sigma)) + square((y - mu) / sigma))) /
      (2 * M_PI * sigma * sigma);
  return val;
}

vector<vector<float>> getImageFltMatrix(Mat &img) {
  int rows = img.rows;
  int cols = img.cols;
  vector<vector<float>> grayScalImg(rows, vector<float>(cols, 0));
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (img.channels() == 1)
        grayScalImg[i][j] = static_cast<int>(img.at<uchar>(i, j));
      if (img.channels() == 3) {
        float val = (0.0722 * (img.at<Vec3b>(i, j)[0])) +
                    (0.7152 * (img.at<Vec3b>(i, j)[1])) +
                    (0.2126 * (img.at<Vec3b>(i, j)[2]));
        grayScalImg[i][j] = val;
      }
    }
  }
  return grayScalImg;
}

void plotImageMatrix(vector<vector<float>> &img) {
  int rows = img.size();
  int cols = img[0].size();
  Mat image(rows, cols, CV_8U);
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      image.at<u_char>(i, j) = static_cast<u_char>((int)(img[i][j]));
    }
  }
  displayImage(image);
}

void plotKeyPointsInImg(Mat &img, vector<pair<int, int>> &points) {
  for (auto point : points) {
    circle(img, Point(point.second, point.first), 3, Scalar(0, 255, 0), 1);
  }
}

void logSubPatchPrint(const vector<vector<float>> &img) {
  if (img.size() > 9 and img[0].size() > 9) {
    cout << "START : ====================\n\n";
    for (int i = 1; i <= subPatchSize; i++) {
      for (int j = 1; j <= subPatchSize; j++) {
        cout << img[i][j] << " ";
      }
      cout << "\n";
    }
    cout << "\nEND : ====================\n\n";
  }
}

#endif