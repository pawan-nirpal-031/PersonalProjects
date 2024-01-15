#ifndef GUASSIAN_TRANSFORM
#define GUASSIAN_TRANSFORM

#include "../Utils/Kernals/GaussianKernal.h"

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

#endif