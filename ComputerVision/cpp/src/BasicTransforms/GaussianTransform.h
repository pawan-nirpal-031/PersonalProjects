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

static Mat guassianFilterTransform(Mat &img, double Sigma) {
  vector<vector<double>> filter = getGuassianKernal(Sigma);
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