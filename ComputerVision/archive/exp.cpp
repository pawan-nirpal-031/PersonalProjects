/*
 compile : g++ exp.cpp -I/usr/local/include/opencv4 -Wl,-rpath,/usr/local/lib /usr/local/lib/libopencv_highgui.so.4.8.0 /usr/local/lib/libopencv_ml.so.4.8.0 /usr/local/lib/libopencv_objdetect.so.4.8.0 /usr/local/lib/libopencv_photo.so.4.8.0 /usr/local/lib/libopencv_stitching.so.4.8.0 /usr/local/lib/libopencv_video.so.4.8.0 /usr/local/lib/libopencv_videoio.so.4.8.0 /usr/local/lib/libopencv_imgcodecs.so.4.8.0 /usr/local/lib/libopencv_calib3d.so.4.8.0 /usr/local/lib/libopencv_dnn.so.4.8.0 /usr/local/lib/libopencv_features2d.so.4.8.0 /usr/local/lib/libopencv_flann.so.4.8.0 /usr/local/lib/libopencv_imgproc.so.4.8.0 /usr/local/lib/libopencv_core.so.4.8.0 -lm -o exp
*/

#include <algorithm>
#include <math.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
using namespace std;
using namespace cv;

void displayImage(Mat &img, unsigned int time = 0, string title = "frame") {
  imshow(title, img);
  waitKey(time);
}

bool isValidPoint(Mat &img, int x, int y) {
  int rows = img.rows;
  int cols = img.cols;
  return (x >= 0 and x < rows and y >= 0 and y < cols);
}

static double square(double x) { return x * x; }

static double computeGaussianFunc(double x, double y, double mu, double sigma) {
  double val =
      exp(-0.5 * ((square((x - mu) / sigma)) + square((y - mu) / sigma))) /
      (2 * M_PI * sigma * sigma);
  return val;
}

static vector<vector<double>> getGuassianKernal(double sigma) {
  int size = 2 * ceil(3 * sigma) + 1;
  vector<vector<double>> Kernal(size, vector<double>(size, 0.0));
  double sum = 0.0;
  int center = size / 2;
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      // x and y removed here, not required because computeGaussianFunc already handles (i - center) (j - center)
      Kernal[i][j] = computeGaussianFunc(i, j, center, sigma);
      sum += Kernal[i][j];
    }
  }
  if (sum != 0) {
    for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
        Kernal[i][j] /= sum;
      }
    }
  }
  return Kernal;
}

static Mat guassianFilterTransformExp(Mat &img, double Sigma) {
  vector<vector<double>> filter = getGuassianKernal(Sigma);
  int FiltSize = filter.size();
  int trows = img.rows - FiltSize + 1;
  int tcols = img.cols - FiltSize + 1;
  // Final output
  Mat transformed(trows, tcols, CV_8U);
  for (int i = 0; i < trows; i++) {
    for (int j = 0; j <tcols; j++) {
      double tval = 0;
      for (int x = 0; x <FiltSize; x++) {
        for (int y = 0; y <FiltSize; y++) {
          tval = tval + (filter[x][y] *
                         static_cast<double>(img.at<u_char>(i + x, j + y)));
          tval = min(tval,255.0);
          tval = max(tval,0.0);
        }
      }
      transformed.at<u_char>(i,j) = static_cast<u_char>(round(tval));
    }
  }
  return (transformed);
}


int main() {
  string imPath =
      "/home/panirpal/workspace/Projects/ComputerVision/data/images/chess2.jpg";
  Mat img = imread(imPath, IMREAD_GRAYSCALE);
  if (!img.empty()) {
    displayImage(img);
    Mat out = guassianFilterTransformExp(img,1.5);
    displayImage(out);
  } else
    cerr << "image not found! exiting...";
  return 0;
}
