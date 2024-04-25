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

unsigned subPatchSize = 5;

void displayImage(Mat &img, unsigned int time = 0, string title = "frame") {
  imshow(title, img);
  waitKey(time);
}

static float square(float x) { return x * x; }

static float computeGaussianFunc(float x, float y, float mu, float sigma) {
  float val =
      exp(-0.5 * ((square((x - mu) / sigma)) + square((y - mu) / sigma))) /
      (2 * M_PI * sigma * sigma);
  return val;
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

Mat getCvImage(vector<vector<float>> img){
    int rows = img.size();
    int cols = img[0].size();
    Mat out(rows,cols,CV_8U);
    for(int i =0;i<rows;i++){
        for(int j =0;j<cols;j++){
            out.at<uchar>(i,j) = static_cast<uchar>((int)img[i][j]);
        }
    }
    return out;
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

bool isImgPoint(int rows, int cols, int x, int y) {
  return (x >= 0 and x < rows and y >= 0 and y < cols);
}

vector<vector<float>>
computeGaussianConvolution(pair<vector<vector<float>>, float> &KernalInfo,
                           int size, vector<vector<float>> &img) {
  int rows = img.size();
  int cols = img[0].size();
  vector<vector<float>> res(rows, vector<float>(cols, 0));
  vector<vector<float>> Kernal = KernalInfo.first;
  float normF = KernalInfo.second;
  int hk = size / 2;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      float val = 0.0;
      for (int x = -hk; x <= hk; x++) {
        for (int y = -hk; y <= hk; y++) {
          if (isImgPoint(rows, cols, i + x, j + y)) {
            val += Kernal[x + hk][y + hk] * (img[i + x][j + y]);
          }
        }
      }
      res[i][j] = (val / normF);
    }
  }
  return res;
}

pair<vector<vector<float>>, float> computeKernalInfo(int size, float s) {
  vector<vector<float>> Kernal(size, vector<float>(size, 0.0));
  float sum = 0.0;
  int center = size / 2;
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      Kernal[i][j] = computeGaussianFunc(i, j, center, s);
      sum += Kernal[i][j];
    }
  }
  return {Kernal, sum};
}

vector<vector<float>> computeGussianBlur(vector<vector<float>> &img,
                                          int Ksize,float s) {
  vector<vector<float>> res;
  //int Ksize = 2 * ceil(3 * s) + 1;
  pair<vector<vector<float>>, float> KernalInfo;
  KernalInfo = computeKernalInfo(Ksize, s);
  vector<vector<float>> gaussianFilterd =
      computeGaussianConvolution(KernalInfo, Ksize, img);
  return gaussianFilterd;
}

void checkForSucessiveBlur(Mat &image){
  auto img = getImageFltMatrix(image);
  float s0 = 1;
  float k = 1.414;
  float sig = 0;
  for(int i = 0;i<4;i++){
    if(i>0)
      sig = sig*k;
    else
      sig = s0;
    auto blur = computeGussianBlur(img,3,sig);
    plotImageMatrix(blur);
    logSubPatchPrint(blur);
    img = blur;
  }
}

int main() {
  // string imPath =
  //     "/home/panirpal/workspace/Projects/ComputerVision/data/images/astro1.jpg";
  // Mat img = imread(imPath, IMREAD_GRAYSCALE);
  // if (!img.empty()) {
  //   checkForSucessiveBlur(img);
  // } else
  //   cerr << "image not found! exiting...";
  
  return 0;
}
