#include <algorithm>
#include <math.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

void displayImage(Mat &img, unsigned int time = 0) {
  imshow("frame", img);
  waitKey(time);
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

void guassianAverageFilterTransform(Mat &img) {
  displayImage(img);
  int filtSize = 3;
  vector<vector<int>> filter = {{1, 2, 1}, {2, 4, 2}, {1, 2, 1}};
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
      transformed.at<u_char>(i, j) = static_cast<u_char>(tval / 16);
    }
  }
  displayImage(transformed);
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
      transformed.at<u_char>(i, j) = static_cast<u_char>(tval);
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
      transformed.at<u_char>(i, j) = static_cast<u_char>(tval);
    }
  }
  displayImage(transformed);
}

void medianFilter(Mat &img) {
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
}
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
void convolution(Mat &img) {}

int main() {
  string imPath =
      "/home/panirpal/workspace/Projects/ComputerVision/data/saltpepper.png";
  Mat img = imread(imPath, IMREAD_GRAYSCALE);
  if (!img.empty())
    medianFilter(img);
  else
    cerr << "image not found! exiting...";
  return 0;
}