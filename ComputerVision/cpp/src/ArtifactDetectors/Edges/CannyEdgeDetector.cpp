#ifndef CANNYEDGE
#define CANNYEDGE

#include "../../Utils/Core/Core.h"
#include "../../Utils/Core/Utils.h"
#include "../../BasicTransforms/GaussianTransform.h"

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
          if (x == 0 and y == 0)
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

int main(){
  string imPath =
      "/home/panirpal/workspace/Projects/ComputerVision/data/home.jpg";
  Mat img = imread(imPath, IMREAD_GRAYSCALE);
  if (!img.empty()) {
    Mat out = getCannyEdgesImg(img);
    displayImage(out);
  } else
    cerr << "image not found! exiting...";
  return 0;
}

#endif