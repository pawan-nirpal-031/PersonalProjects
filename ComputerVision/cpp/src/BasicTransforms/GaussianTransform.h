#ifndef GUASSIAN_TRANSFORM
#define GUASSIAN_TRANSFORM

#include "../Utils/Core/Utils.h"
#include "../Utils/Kernals/GaussianKernal.h"

// Think if Filter size = 3 and Sigma = 1.5 is right, See
/*
Here's the formula for computing the kernel size for a Gaussian filter given the
standard deviation (sigma):

Kernel size = 2 * ceil(3 * sigma) + 1

Explanation:

ceil(): This function rounds a number up to the nearest integer.
3 * sigma: This approximates the distance from the center of the Gaussian
distribution to where its tails become negligible.
+ 1: This accounts for the central pixel of the kernel.
Example:

If sigma = 1.5, then:

Kernel size = 2 * ceil(3 * 1.5) + 1 = 2 * ceil(4.5) + 1 = 2 * 5 + 1 = 11

Key Points:

Rule of Thumb: A common rule of thumb is to use a kernel size of at least
6*sigma to ensure adequate coverage of the Gaussian distribution. Trade-offs:
Larger kernel sizes result in smoother filtering but can increase computational
cost. Smaller kernel sizes reduce computation time but may not capture the full
extent of the Gaussian distribution. OpenCV Function: OpenCV provides the
getGaussianKernel function to generate a Gaussian kernel of a specified size and
sigma. Additional Considerations:

Discrete Approximation: Gaussian kernels in image processing are discrete
approximations of the continuous Gaussian function. Edge Effects: Consider edge
handling techniques (e.g., padding) when applying Gaussian filters to avoid
boundary artifacts. Image-Specific Adjustments: The optimal kernel size may vary
depending on image characteristics and desired filtering effects.

*/

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

vector<vector<float>> computeGussianBlur(vector<vector<float>> &img, int Ksize,
                                         float s) {
  if (Ksize <= 0 or s == 0) {
    return img;
  }
  vector<vector<float>> res;
  pair<vector<vector<float>>, float> KernalInfo;
  KernalInfo = computeKernalInfo(Ksize, s);
  vector<vector<float>> gaussianFilterd =
      computeGaussianConvolution(KernalInfo, Ksize, img);
  return gaussianFilterd;
}

static Mat guassianFilterTransform(Mat &img, double Sigma) {
  vector<vector<double>> filter = getGuassianKernalNormalized(Sigma);
  int FiltSize = filter.size();
  int trows = img.rows - FiltSize + 1;
  int tcols = img.cols - FiltSize + 1;
  // Final output
  Mat transformed(trows, tcols, CV_8U);
  for (int i = 0; i < trows; i++) {
    for (int j = 0; j < tcols; j++) {
      double tval = 0;
      for (int x = 0; x < FiltSize; x++) {
        for (int y = 0; y < FiltSize; y++) {
          tval = tval + (filter[x][y] *
                         static_cast<double>(img.at<u_char>(i + x, j + y)));
          tval = min(tval, 255.0);
          tval = max(tval, 0.0);
        }
      }
      transformed.at<u_char>(i, j) = static_cast<u_char>(round(tval));
    }
  }
  return (transformed);
}

#endif