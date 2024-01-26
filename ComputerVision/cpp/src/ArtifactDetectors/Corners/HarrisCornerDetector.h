#ifndef HARRIS_CORNER
#define HARRIS_CORNER

#include "../../BasicTransforms/GaussianTransform.h"
#include "../../Utils/Core/Core.h"
#include "../../Utils/Core/Utils.h"

// This implmn somehow ends up being a really good edge detector. This needs to
// be refactored and fixed as per :
// https://stackoverflow.com/questions/77847011/implementing-harris-corner-detector-by-following-standard-material-it-ends-up/77847391#77847391

// TODO : Improve paramterized implementation. Of this Detector.
class HarrisCornerDetector {
  bool isImgPoint(int rows, int cols, int x, int y) {
    return (x >= 0 and x < rows and y >= 0 and y < cols);
  }

  static float square(float x) { return x * x; }

  static float computeGaussianFunc(float x, float y, float mu, float sigma) {
    float val =
        exp(-0.5 * ((square((x - mu) / sigma)) + square((y - mu) / sigma))) /
        (2 * M_PI * sigma * sigma);
    return val;
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

  vector<vector<float>>
  computeGaussianConv(pair<vector<vector<float>>, float> &KernalInfo, int size,
                      vector<vector<float>> &img) {
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

  vector<vector<float>> getGaussianFilterImg(vector<vector<float>> &img,
                                             int fsize, float s) {
    vector<vector<float>> res;
    pair<vector<vector<float>>, float> KernalInfo;
    if (fsize <= 0) {
      return img;
    }
    KernalInfo = computeKernalInfo(fsize, s);
    vector<vector<float>> gaussianFilterd =
        computeGaussianConv(KernalInfo, fsize, img);
    return gaussianFilterd;
  }

  pair<vector<vector<float>>, vector<vector<float>>>
  computeSobelGradients(vector<vector<float>> &img) {
    int rows = img.size();
    int cols = img[0].size();
    vector<vector<int>> filtery, filterx;
    int filtSize = 3;
    int hf = filtSize / 2;
    filtery = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    filterx = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    vector<vector<float>> Gx(rows, vector<float>(cols, 0)),
        Gy(rows, vector<float>(cols, 0));
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        double gradX = 0;
        double gradY = 0;
        // Compute Sobel Gradients.
        for (int x = -hf; x <= hf; x++) {
          for (int y = -hf; y <= hf; y++) {
            if (isImgPoint(rows, cols, i + x, j + y)) {
              gradX = gradX + (filterx[x + hf][y + hf] * img[i + x][j + y]);
              gradY = gradY + (filtery[x + hf][y + hf] * img[i + x][j + y]);
            }
          }
        }
        Gx[i][j] = gradX;
        Gy[i][j] = gradY;
      }
    }
    return {Gx, Gy};
  }

  pair<vector<vector<float>>, float> computeStrResponseMat(
      pair<vector<vector<float>>, vector<vector<float>>> &gradients, float k,
      int ws) {
    vector<vector<float>> &Gx = gradients.first;
    vector<vector<float>> &Gy = gradients.second;
    vector<vector<float>> resMat(Gx.size(), vector<float>(Gx[0].size(), 0));
    float maxRes = FLT_MIN;
    int hw = ws / 2;
    for (int i = 0; i < Gx.size(); i++) {
      for (int j = 0; j < Gx[0].size(); j++) {
        float Gxx = 0.0;
        float Gyy = 0.0;
        float Gxy = 0.0;
        for (int x = -hw; x <= hw; x++) {
          for (int y = -hw; y <= hw; y++) {
            if (isImgPoint(Gx.size(), Gx[0].size(), i + x, j + y)) {
              Gxx += (Gx[i + x][j + y] * Gx[i + x][j + y]);
              Gyy += (Gy[i + x][j + y] * Gy[i + x][j + y]);
              Gxy += (Gx[i + x][j + y] * Gy[i + x][j + y]);
            }
          }
        }
        double det = Gxx * Gyy - Gxy * Gxy;
        double trace = Gxx + Gyy;
        float resF = (det - k * (trace * trace));
        resMat[i][j] = resF;
        if (resF > maxRes)
          maxRes = resF;
      }
    }
    return {resMat, maxRes};
  }

  vector<pair<int, int>>
  computeCornerPoints(pair<vector<vector<float>>, float> &responseMat) {
    int nmsWs = 3;
    float nmsTh = 0.02;
    float threshold = responseMat.second * nmsTh;
    vector<vector<float>> resMat = responseMat.first;
    vector<pair<int, int>> cornerPts;
    for (int i = 0; i < resMat.size(); i++) {
      for (int j = 0; j < resMat[0].size(); j++) {
        float localMaxima = FLT_MIN;
        pair<int, int> resPts;
        for (int x = 0; x < nmsWs; x++) {
          for (int y = 0; y < nmsWs; y++) {
            if (isImgPoint(resMat.size(), resMat[0].size(), i + x, j + y)) {
              if (resMat[i + x][j + y] > localMaxima) {
                localMaxima = resMat[i + x][j + y];
                resPts = make_pair(i + x, j + y);
              }
            }
          }
        }
        if (localMaxima >= threshold) {
          cornerPts.push_back(resPts);
        }
      }
    }
    return cornerPts;
  }

  vector<pair<int, int>> mapCornerPtsToImg(vector<pair<int, int>> &cornerPts) {
    vector<pair<int, int>> imgCord;
    for (auto &tuple : cornerPts)
      // Point shifting for sobel gradients.
      imgCord.push_back({tuple.first + 2, tuple.second + 2});

    return imgCord;
  }

  vector<pair<int, int>> detectHarrisCornersDriver(Mat &img) {
    pair<vector<vector<float>>, vector<vector<float>>> gradients;
    int rows = img.rows;
    int cols = img.cols;
    vector<vector<float>> grayScalImg(rows, vector<float>(cols, 0)),
        gaussFiltImg;
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
    gaussFiltImg =
        getGaussianFilterImg(grayScalImg, this->GaussKernalSize, this->sigma);
    gradients = computeSobelGradients(gaussFiltImg);
    pair<vector<vector<float>>, float> responseMat =
        computeStrResponseMat(gradients, this->k, this->GaussKernalSize);
    vector<pair<int, int>> cornerPts = computeCornerPoints(responseMat);
    vector<pair<int, int>> imgCorners = mapCornerPtsToImg(cornerPts);
    return imgCorners;
  }

public:
  float k;
  int GaussKernalSize;
  float sigma;
  HarrisCornerDetector() {
    k = 0.04;
    GaussKernalSize = 3;
    sigma = 1;
  }
  HarrisCornerDetector(int GaussianKernalSzG, float kG, float sigmaG) {
    k = kG;
    GaussKernalSize = GaussianKernalSzG;
    sigma = sigmaG;
  }
  void detectHarrisCorners(Mat img) {
    vector<pair<int, int>> points = detectHarrisCornersDriver(img);
    for (auto point : points) {
      circle(img, Point(point.second, point.first), 3, Scalar(0, 255, 0), 1);
    }
  }
};
#endif