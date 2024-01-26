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

static double getDoubleValOf(Mat &img, int x, int y) {
  return static_cast<double>(img.at<uchar>(x, y));
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

static Mat guassianFilterTransform(Mat &img, double Sigma) {
  vector<vector<double>> filter = getGuassianKernal(Sigma);
  int FiltSize = filter.size();
  int trows = img.rows - FiltSize + 1;
  int tcols = img.cols - FiltSize + 1;
  // Final output
  Mat transformed(trows, tcols, CV_64F);
  for (int i = 0; i < trows; i++) {
    for (int j = 0; j <tcols; j++) {
      double tval = 0;
      for (int x = 0; x <FiltSize; x++) {
        for (int y = 0; y <FiltSize; y++) {
          tval = tval + (filter[x][y] * (img.at<double>(i + x, j + y)));
          //tval = min(tval,255.0);
          //tval = max(tval,0.0);
        }
      }
      transformed.at<double>(i,j) = (tval);
    }
  }
  return (transformed);
}

double computeCornerResponseFunc(vector<vector<double>> &StructMat) {
  double det =
      StructMat[0][0] * StructMat[1][1] - StructMat[0][1] * StructMat[1][0];
  double trace = StructMat[0][0] + StructMat[1][1];
  return (det - (0.05) * (trace * trace));
}


Mat computeStructureTensorMatrixAndCornerResponse(Mat &GradXSqSmooth,
                                                  Mat &GradYSqSmooth,
                                                  Mat &GradXYSqSmooth) {
  int rows = GradXSqSmooth.rows;
  int cols = GradXSqSmooth.cols;
  Mat ResponseMat(rows, cols, CV_64F);
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      vector<vector<double>> STM = {{getDoubleValOf(GradXSqSmooth, i, j),
                                     getDoubleValOf(GradXYSqSmooth, i, j)},
                                    {getDoubleValOf(GradXYSqSmooth, i, j),
                                     getDoubleValOf(GradYSqSmooth, i, j)}};
      double CornerRes = computeCornerResponseFunc(STM);
      ResponseMat.at<double>(i, j) = CornerRes;
    }
  }
  return ResponseMat;
}


// Try to Optimize for GradX this is not required we can directly compute
// GradXSq.
Mat detectHarrisCornersUpd(Mat &img) {
  int filtSize = 3;
  vector<vector<int>> filtery, filterx;
  filtery = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
  filterx = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
  int trows = img.rows - filtSize + 1;
  int tcols = img.cols - filtSize + 1;
  Mat GradX(trows, tcols, CV_64F);
  Mat GradY(trows, tcols, CV_64F);
  Mat gradDir(trows, tcols, CV_64F);
  Mat result(trows, tcols, CV_8U);
  double Thrs = 200;
  /*
    1) Derivative Calculation:

    Calculate the horizontal (Ix) and vertical (Iy) derivatives of the grayscale
    image using the Sobel operator: Ix = Sobel(I_gray, dx) Iy = Sobel(I_gray,
    dy) dx and dy are the Sobel operators in the x and y directions,
    respectively.
  */
  for (int i =0; i <trows; i++) {
    for (int j = 0; j <tcols; j++) {
      double gradX = 0;
      double gradY = 0;
      // Compute Sobel Gradients.
      for (int x = 0; x <filtSize; x++) {
        for (int y = 0; y < filtSize; y++) {
          gradX = gradX + (filterx[x][y] *
                           static_cast<double>(img.at<uchar>(i + x, j + y)));
          gradY = gradY + (filtery[x][y] *
                           static_cast<double>(img.at<uchar>(i + x, j + y)));
        }
      }
      GradX.at<double>(i, j) = gradX;
      GradY.at<double>(i, j) = gradY;
      // GradDir required for non maximum supression
      double tval = sqrt(gradX * gradX + gradY * gradY);
      if (gradX == 0.00) {
        gradDir.at<double>(i, j) = (gradY >= 0 ? M_PI / 2.0 : -M_PI / 2.0);
      } else {
        gradDir.at<double>(i, j) = atan(gradY / gradX);
      }
      gradDir.at<double>(i, j) *= (180.0 / M_PI);
      gradDir.at<double>(i, j) = abs(gradDir.at<double>(i, j));
    }
  }
  /*
    3. Derivative Products:
    Element wise product
    Ix2 = Ix * Ix
    Iy2 = Iy * Iy
    Ixy = Ix * Iy
  */
  Mat GradXSq(trows, tcols, CV_64F);
  Mat GradYSq(trows, tcols, CV_64F);
  Mat GradXY(trows, tcols, CV_64F);
  for (int i = 0; i < trows; i++) {
    for (int j = 0; j < tcols; j++) {
      GradXSq.at<double>(i, j) =
          GradX.at<double>(i, j) * GradX.at<double>(i, j);
      GradYSq.at<double>(i, j) =
          GradY.at<double>(i, j) * GradY.at<double>(i, j);
      GradXY.at<double>(i, j) = GradX.at<double>(i, j) * GradY.at<double>(i, j);
    }
  }
  /*
    Gaussian Smoothing:

    Apply Gaussian smoothing to each of the derivative products to reduce noise
    sensitivity: Ix2_smooth = GaussianBlur(Ix2, kernel_size) Iy2_smooth =
    GaussianBlur(Iy2, kernel_size) Ixy_smooth = GaussianBlur(Ixy, kernel_size)
  */
  displayImage(GradXSq);
  Mat GradXSqSmooth = guassianFilterTransform(GradXSq, 1.3);
  displayImage(GradXSqSmooth);
  Mat GradYSqSmooth = guassianFilterTransform(GradYSq, 1.3);
  Mat GradXYSqSmooth = guassianFilterTransform(GradXY, 1.3);
  Mat StructTensorMat = computeStructureTensorMatrixAndCornerResponse(
      GradXSqSmooth, GradYSqSmooth, GradXYSqSmooth);
  return StructTensorMat;
}


int main() {
  string imPath =
      "/home/panirpal/workspace/Projects/ComputerVision/data/images/chess2.jpg";
  Mat img = imread(imPath, IMREAD_GRAYSCALE);
  if (!img.empty()) {
    displayImage(img);
    //Mat gaussian = guassianFilterTransform(img,1.3);
    Mat out = detectHarrisCornersUpd(img);
    //displayImage(out);
  } else
    cerr << "image not found! exiting...";
  return 0;
}
