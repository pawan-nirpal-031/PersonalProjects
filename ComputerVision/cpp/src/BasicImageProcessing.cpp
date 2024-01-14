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

vector<vector<double>> getGuassianKernal(int size, double sigma) {
  vector<vector<double>> Kernal(size, vector<double>(size, 0.0));
  double sum = 0.0;
  int center = size / 2;
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      int x = i - center;
      int y = j - center;
      Kernal[i][j] = exp(-(x * x + y * y) / (2 * sigma * sigma));
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

void Transform1(Mat &image) {
  displayImage(image);
  for (int y = 0; y < image.rows; ++y) {
    for (int x = 0; x < image.cols; ++x) {
      cv::Vec3b &pixel = image.at<cv::Vec3b>(y, x);
      pixel[0] += 20;
      pixel[1] += 20;
      pixel[2] += 20;
    }
  }
  displayImage(image);
}

void inverionAlongYAxisTransform(Mat &img) { // inversion
  displayImage(img);
  for (int j = 0; j < img.cols / 2; j++) {
    for (int i = 0; i < img.rows; i++) {
      swap(img.at<cv::Vec3b>(i, j), img.at<cv::Vec3b>(i, img.cols - 1 - j));
    }
  }
  displayImage(img);
}

void pixelInverseTransform(Mat &image) {
  displayImage(image);
  for (int y = 0; y < image.rows; ++y) {
    for (int x = 0; x < image.cols; ++x) {
      cv::Vec3b &pixel = image.at<cv::Vec3b>(y, x);
      pixel[0] = 255 - pixel[0];
      pixel[1] = 255 - pixel[1];
      pixel[2] = 255 - pixel[2];
    }
  }
  displayImage(image);
}

void linearContrastStreachingTransform(Mat &img) { // linear contrast streching
  displayImage(img);
  u_char maxVal = 0;
  u_char minVal = 255;
  for (int i = 0; i < img.rows; i++) {
    for (int j = 0; j < img.cols; j++) {
      maxVal = max(maxVal, img.at<u_char>(i, j));
      minVal = min(minVal, img.at<u_char>(i, j));
    }
  }
  for (int i = 0; i < img.rows; i++) {
    for (int j = 0; j < img.cols; j++) {
      img.at<u_char>(i, j) =
          (img.at<u_char>(i, j) - minVal) * (255 / (maxVal - minVal));
    }
  }
  displayImage(img);
}

// Blur effect transformation
void movingAverageTransform(Mat &img, int winSize = 3) {
  displayImage(img);
  int trows = img.rows - winSize + 1;
  int tcols = img.cols - winSize + 1;
  Mat integralImg(img.rows, img.cols, CV_32S);

  integralImg.at<int>(0, 0) = static_cast<int>(img.at<u_char>(0, 0));
  for (int i = 1; i < img.cols; i++)
    integralImg.at<int>(0, i) =
        static_cast<int>(img.at<u_char>(0, i)) + integralImg.at<int>(0, i - 1);

  for (int i = 1; i < img.rows; i++)
    integralImg.at<int>(i, 0) =
        static_cast<int>(img.at<u_char>(i, 0)) + integralImg.at<int>(i - 1, 0);

  for (int i = 1; i < img.rows; i++) {
    for (int j = 1; j < img.cols; j++) {
      integralImg.at<int>(i, j) = static_cast<int>(img.at<u_char>(i, j)) +
                                  integralImg.at<int>(i - 1, j) +
                                  integralImg.at<int>(i, j - 1) -
                                  integralImg.at<int>(i - 1, j - 1);
    }
  }

  Mat transformed(trows, tcols, IMREAD_GRAYSCALE);
  for (int i = 0; i < trows; i++) {
    for (int j = 0; j < tcols; j++) {
      int BrSubMat = integralImg.at<int>(i + winSize - 1, j + winSize - 1);
      if (i >= 1)
        BrSubMat -= integralImg.at<int>(i - 1, j + winSize - 1);
      if (j >= 1)
        BrSubMat -= integralImg.at<int>(i + winSize - 1, j - 1);
      if (i >= 1 and j >= 1)
        BrSubMat += integralImg.at<int>(i - 1, j - 1);
      transformed.at<u_char>(i, j) = (BrSubMat / (winSize * winSize));
    }
  }

  displayImage(transformed);
}

void histogramEqualization(Mat &img) {
  displayImage(img);
  // map of intensity i : count of i, ie histogram of image
  vector<int> histogram(256, 0);
  for (int i = 0; i < img.rows; i++) {
    for (int j = 0; j < img.cols; j++)
      histogram[static_cast<int>(img.at<u_char>(i, j))] += 1;
  }
  // compute cumulative distribution function this is just prefix sums of
  // histogram
  vector<int> cumulativeDistFunc(256, 0);
  cumulativeDistFunc[0] = histogram[0];
  int minCdf = cumulativeDistFunc[0];
  int numPixels = img.rows * img.cols;
  for (int i = 1; i < 256; i++)
    cumulativeDistFunc[i] = cumulativeDistFunc[i - 1] + histogram[i];
  for (int i = 1; i < 256; i++)
    cumulativeDistFunc[i] =
        (int)(((float)(cumulativeDistFunc[i] - minCdf) / (numPixels - 1)) *
              255);

  int trows = img.rows;
  int tcols = img.cols;
  Mat transformed(trows, tcols, IMREAD_GRAYSCALE);
  for (int i = 0; i < trows; i++) {
    for (int j = 0; j < tcols; j++) {
      int pixelVal = static_cast<int>(img.at<u_char>(i, j));
      int tpixelVal = cumulativeDistFunc[pixelVal];
      transformed.at<u_char>(i, j) = static_cast<u_char>(tpixelVal);
    }
  }
  displayImage(transformed);
}

Mat guassianFilterTransform(Mat &img, int FiltSize = 3, double Sigma = 1.5) {
  vector<vector<double>> filter = getGuassianKernal(FiltSize, Sigma);
  int trows = img.rows - FiltSize + 1;
  int tcols = img.cols - FiltSize + 1;
  Mat transformed(trows, tcols, CV_8U);
  for (int i = 1; i <= trows - 2; i++) {
    for (int j = 1; j <= tcols - 2; j++) {
      double tval = 0;
      for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
          tval = tval + (filter[x + 1][y + 1] *
                         static_cast<double>(img.at<u_char>(i + x, j + y)));
        }
      }
      transformed.at<u_char>(i, j) = static_cast<u_char>(tval);
    }
  }
  return (transformed);
}

void verticalEdgeFilterTransform(Mat &img) {
  displayImage(img);
  int filtSize = 3;
  vector<vector<int>> filter = {{-1, 0, 1}, {-1, 0, 1}, {-1, 0, 1}};
  int trows = img.rows - filtSize + 1;
  int tcols = img.cols - filtSize + 1;
  Mat transformed(trows, tcols, IMREAD_GRAYSCALE);
  for (int i = 1; i <= trows - 2; i++) {
    for (int j = 1; j <= tcols - 2; j++) {
      int tval = 0;
      for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
          tval = tval + (filter[x + 1][y + 1] *
                         static_cast<int>(img.at<u_char>(i + x, j + y)));
        }
      }
      transformed.at<u_char>(i, j) = static_cast<u_char>(tval + 255);
    }
  }
  displayImage(transformed);
}

void horizontalEdgeFilterTransform(Mat &img) {
  displayImage(img);
  int filtSize = 3;
  vector<vector<int>> filter = {{-1, -1, -1}, {0, 0, 0}, {1, 0, 1}};
  int trows = img.rows - filtSize + 1;
  int tcols = img.cols - filtSize + 1;
  Mat transformed(trows, tcols, IMREAD_GRAYSCALE);
  for (int i = 1; i <= trows - 2; i++) {
    for (int j = 1; j <= tcols - 2; j++) {
      int tval = 0;
      for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
          tval = tval + (filter[x + 1][y + 1] *
                         static_cast<int>(img.at<u_char>(i + x, j + y)));
        }
      }
      transformed.at<u_char>(i, j) = static_cast<u_char>(tval + 255);
    }
  }
  displayImage(transformed);
}

Mat medianFilter(Mat &img) {
  displayImage(img);
  int filtSize = 3;
  int trows = img.rows - filtSize + 1;
  int tcols = img.cols - filtSize + 1;
  Mat transformed(trows, tcols, IMREAD_GRAYSCALE);
  for (int i = 1; i <= trows - 2; i++) {
    for (int j = 1; j <= tcols - 2; j++) {
      vector<int> median;
      for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
          median.push_back(static_cast<int>(img.at<u_char>(i + x, j + y)));
        }
      }
      sort(median.begin(), median.end());
      transformed.at<u_char>(i, j) = static_cast<u_char>(median[4]);
    }
  }
  displayImage(transformed);
  return transformed;
}

// Fourier transform vairent fast fourier transform can perform convolution
// opreation of nxn image in n(logn) time this is the fastest method for
// convolution opreation. It is done using divide and conquer recursive
// algorithm. Implement convolution using fft. Similarlly corelation as well.
// vector<int> getValueRepresenation(vector<int> CoeffRep){
//   int n = CoeffRep.size();
//   if(n==1)
//     return CoeffRep;
//   double w = exp()
// }

// Impulse signal double flips on filter application. Convolution nullifies this
// double flip.  T(i,j)= $u$v (H(u,v)*I(i-u,j-u))  where $ is summation opreator
// T is the transformed image, H is the filter, and I is the input image. So
// convolution is cross correleation of double flipped input. This are linear
// shift invarient opreator, Linearity means Image I with filters h0 and h1 when
// applied are distributive ie, I(h0 + h1) = I*h0 + I*h1, also known as the
// super position principle. The other property is called shift invarience :
// shifting (or translating) a signal commutes with applying the opreator.
// T(i,j) = H(i+k,j+l) -> f(g(i,j)) = f(h(i+k,j+l)), In layman terms the effect
// of the opreator is the same no matter where in the image, ie it is invarient
// of the image location. Convolution (denoted by * opreator) is commutative a*b
// = b*a ( no diff btwn filter and signal/img) a*(b*c) = (a*b)*c helps in
// precomputations and reducing computation time, scalar : ka*b = a*kb =
// k*(a*b),Identity : Unit impulse. An important optimization uses property of
// seperablity usually for image of size nxn, and a filter size of k (usually 3)
// we require k^2 x n^2 computatations over the entire image, making it very
// expensive, seperablity allows us in some cases to bring this computation down
// to (2k * n^2) by performing 1d horizontal convolution followd by 1d vertial
// convolution having the same effect as the k^2 computations per pixel at end.
void convolutionOptimized(Mat &img) {}

// Implement canny edge detector
/*
  1) filter image with derivate of guassian
  2) find magnitude and derection of the gradient
  3) apply non max supression
  4) linking and thresholding : use high and low threshold use high threshold to
  say edge and low threshold to continue them, if a pixel is connected to an
  edge pixel than it an edge pixek too.
*/
// Returns Magnitude of Gradient and direction of Gradient.
Mat sobelFilterEdgeTransformWithNonMaxSupression(Mat &img) {
  int filtSize = 3;
  vector<vector<int>> filtery, filterx;
  filtery = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
  filterx = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
  int trows = img.rows - filtSize + 1;
  int tcols = img.cols - filtSize + 1;
  Mat gradMag(trows, tcols, CV_8U);
  Mat gradDir(trows, tcols, CV_64F);
  for (int i = 1; i <= trows - 2; i++) {
    for (int j = 1; j <= tcols - 2; j++) {
      double tvalx = 0;
      double tvaly = 0;
      for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
          tvalx = tvalx + (filterx[x + 1][y + 1] *
                           static_cast<double>(img.at<u_char>(i + x, j + y)));
          tvaly = tvaly + (filtery[x + 1][y + 1] *
                           static_cast<double>(img.at<u_char>(i + x, j + y)));
        }
      }
      double tval = sqrt(tvalx * tvalx + tvaly * tvaly);
      if (tvalx == 0.00) {
        gradDir.at<double>(i, j) = (tvaly >= 0 ? M_PI / 2.0 : -M_PI / 2.0);
      } else {
        gradDir.at<double>(i, j) = atan(tvaly / tvalx);
      }
      gradDir.at<double>(i, j) *= (180.0 / M_PI);
      gradDir.at<double>(i, j) = abs(gradDir.at<double>(i, j));
      gradMag.at<u_char>(i, j) = static_cast<u_char>(tval);
    }
  }
  Mat finalImg(trows, tcols, CV_8U);
  for (int i = 1; i < trows - 1; i++) {
    for (int j = 1; j < tcols - 1; j++) {
      double angle = gradDir.at<double>(i, j);
      double neigh1, neigh2;
      if ((angle >= 0 and angle < 22.5) or (angle >= 157.5 && angle < 180)) {
        neigh1 = gradMag.at<u_char>(i, j + 1);
        neigh2 = gradMag.at<u_char>(i, j - 1);
      } else if ((angle >= 22.5 and angle < 67.5)) {
        neigh1 = gradMag.at<u_char>(i - 1, j + 1);
        neigh2 = gradMag.at<u_char>(i + 1, j - 1);
      } else if ((angle >= 67.5 and angle < 112.5)) {
        neigh1 = gradMag.at<u_char>(i - 1, j);
        neigh2 = gradMag.at<u_char>(i + 1, j);
      } else if ((angle >= 112.5 and angle < 157.5)) {
        neigh1 = gradMag.at<u_char>(i - 1, j - 1);
        neigh2 = gradMag.at<u_char>(i + 1, j + 1);
      } else {
        neigh1 = neigh2 = 0;
      }

      if (gradMag.at<u_char>(i, j) >= max(neigh1, neigh2))
        finalImg.at<u_char>(i, j) = gradMag.at<u_char>(i, j);
      else
        finalImg.at<u_char>(i, j) = 0;
    }
  }
  return finalImg;
}

pair<int, int> getDoubleThresholds(Mat &img) {
  int rows = img.rows;
  int cols = img.cols;
  vector<int> frequency(256, 0);
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      frequency[static_cast<int>(img.at<u_char>(i, j))] += 1;
    }
  }
  int upperI = 0;
  int upperF = 0;
  for (int i = 210; i <= 240; i++) {
    if (frequency[i] > upperF) {
      upperF = frequency[i];
      upperI = i;
    }
  }
  int lowerI = 0;
  int lowerF = INT_MAX;
  for (int i = 40; i <= 60; i++) {
    if (frequency[i] < lowerF) {
      lowerF = frequency[i];
      lowerI = i;
    }
  }
  return {upperI, lowerI};
}

bool isStrongEdgePoint(int x, int y, Mat &img) {
  return (static_cast<int>(img.at<u_char>(x, y)) == 255);
}

bool isWeakEdgePoint(int x, int y, Mat &img) {
  return (static_cast<int>(img.at<u_char>(x, y)) == 128);
}

void edgeTracking(int u, int v, Mat &img) {
  int rows = img.rows;
  int cols = img.cols;
  if (static_cast<int>(img.at<u_char>(u, v)) == 128) {
    bool isConnectedToStrongEdge = false;
    for (int x = -1; x <= 1; x++) {
      for (int y = -1; y <= 1; y++) {
        if (isValidPoint(img, u + x, v + y) and
            isStrongEdgePoint(u + x, v + y, img)) {
          isConnectedToStrongEdge = true;
        }
      }
    }
    if (isConnectedToStrongEdge) {
      img.at<u_char>(u, v) = 255;
      for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
          if(x==0 and y==0)
            continue;
          if (isValidPoint(img, u + x, v + y) and
              isWeakEdgePoint(u + x, v + y, img)) {
            edgeTracking(u + x, v + y, img);
          }
        }
      }
    }
  }
}

void performDiff(Mat img1, Mat img2) {
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

Mat getCannyEdgesImg(Mat &img) {
  Mat guassian = guassianFilterTransform(img);
  Mat sobelAndNMSImg = sobelFilterEdgeTransformWithNonMaxSupression(guassian);
  int rows = sobelAndNMSImg.rows;
  int cols = sobelAndNMSImg.cols;
  pair<int, int> thresholds = getDoubleThresholds(sobelAndNMSImg);
  int upperI = thresholds.first;
  int lowerI = thresholds.second;
  vector<pair<int, int>> weakEds;
  Mat result(rows, cols, CV_8U);
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      double mag = static_cast<double>(sobelAndNMSImg.at<u_char>(i, j));
      if (mag >= upperI) {
        result.at<u_char>(i, j) = 255;
      } else if (mag >= lowerI) {
        result.at<u_char>(i, j) = 128;
        weakEds.push_back({i, j});
      } else {
        result.at<u_char>(i, j) = 0;
      }
    }
  }
  for (auto point : weakEds) {
    edgeTracking(point.first, point.second, result);
  }
  return result;
}

int main() {
  string imPath =
      "/home/panirpal/workspace/Projects/ComputerVision/data/frm.png";
  Mat img = imread(imPath, IMREAD_GRAYSCALE);
  if (!img.empty()) {
    Mat out = getCannyEdgesImg(img);
    displayImage(out);
  } else
    cerr << "image not found! exiting...";
  return 0;
}