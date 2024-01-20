#ifndef HARRIS_CORNER
#define HARRIS_CORNER

#include "../../BasicTransforms/GaussianTransform.h"
#include "../../Utils/Core/Core.h"
#include "../../Utils/Core/Utils.h"

double computeCornerResponseFunc(vector<vector<double>> &StructMat) {
  double det =
      StructMat[0][0] * StructMat[1][1] - StructMat[0][1] * StructMat[1][0];
  double trace = StructMat[0][0] + StructMat[1][1];
  return (det + (0.04) * (trace * trace));
}

Mat detectHarrisCorners(Mat &img) {
  int filtSize = 3;
  vector<vector<int>> filtery, filterx;
  filtery = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
  filterx = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
  int trows = img.rows - filtSize + 1;
  int tcols = img.cols - filtSize + 1;
  Mat gradMag(trows, tcols, CV_8U);
  Mat gradDir(trows, tcols, CV_64F);
  Mat result(trows, tcols, CV_8U);
  double Thrs = 200;
  vector<vector<double>> gaussianKernal = getGuassianKernal(3, 1.5);
  for (int i = 1; i <= trows - 2; i++) {
    for (int j = 1; j <= tcols - 2; j++) {
      double gradX = 0;
      double gradY = 0;
      for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
          gradX =
              gradX + gaussianKernal[x + 1][y + 1] *
                          (filterx[x + 1][y + 1] *
                           static_cast<double>(img.at<u_char>(i + x, j + y)));
          gradY =
              gradY + gaussianKernal[x + 1][y + 1] *
                          (filtery[x + 1][y + 1] *
                           static_cast<double>(img.at<u_char>(i + x, j + y)));
        }
      }
      vector<vector<double>> strtMtrx(2, vector<double>(2, 0.0));
      strtMtrx[0][0] = gradX * gradX;
      strtMtrx[1][1] = gradY * gradY;
      strtMtrx[0][1] = strtMtrx[1][0] = gradX * gradY;
      double CornerResponseFunc = computeCornerResponseFunc(strtMtrx);
      if (CornerResponseFunc >= Thrs) {
        result.at<u_char>(i, j) = 255;
      }
      double tval = sqrt(gradX * gradX + gradY * gradY);
      if (gradX == 0.00) {
        gradDir.at<double>(i, j) = (gradY >= 0 ? M_PI / 2.0 : -M_PI / 2.0);
      } else {
        gradDir.at<double>(i, j) = atan(gradY / gradX);
      }
      gradDir.at<double>(i, j) *= (180.0 / M_PI);
      gradDir.at<double>(i, j) = abs(gradDir.at<double>(i, j));
      gradMag.at<u_char>(i, j) = static_cast<u_char>(min(255.0, tval));
    }
  }
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
        result.at<u_char>(i, j) = result.at<u_char>(i, j);
      else
        result.at<u_char>(i, j) = 0;
    }
  }
  return result;
}

#endif