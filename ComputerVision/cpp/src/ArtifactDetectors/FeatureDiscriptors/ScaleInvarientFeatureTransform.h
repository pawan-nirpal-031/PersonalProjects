#ifndef SIFT_H
#define SIFT_H

#include "../../BasicTransforms/GaussianTransform.h"
#include "../../Utils/Core/Core.h"
#include "../../Utils/Core/Utils.h"
// SIFT transforms image data into scale invarient image points. Extract key
// points and effective way of describing them. Image information is translated
// to local features which are invarient of scale, rotation, illumination and
// shear(perspective).

namespace MvSIFT {

class SIFT {
  float k;
  int numOctaves;
  int scalesPerOct;
  float initalSigma;
  int gKernelSize;

  bool boundsCheck(int x, int y, int r, int c) {
    return (x >= 0 and x + 1 < r and y >= 0 and y + 1 < c);
  }

  vector<vector<float>> downsampleImg(vector<vector<float>> &currentImg) {
    int rows = currentImg.size();
    int cols = currentImg[0].size();
    vector<vector<float>> output(rows / 2, vector<float>(cols / 2, 0));
    for (int i = 0; i < rows; i += 2) {
      for (int j = 0; j < cols; j += 2) {
        if (not boundsCheck(i, j, rows, cols))
          continue;
        float val = 0;
        for (int v = 0; v <= 1; v++) {
          for (int w = 0; w <= 1; w++) {
            val += currentImg[i + v][j + w];
          }
        }
        output[i / 2][j / 2] = val / 4;
      }
    }
    return output;
  }

  vector<vector<vector<vector<float>>>>
  computeScaleSpacePyramid(vector<vector<float>> img) {
    vector<vector<vector<vector<float>>>> scaleSpacePyramid;
    vector<vector<float>> currentImg = img;
    for (int oct = 1; oct <= numOctaves; oct++) {
      vector<vector<vector<float>>> octImages;
      float sigma = initalSigma;
      for (int scl = 1; scl <= scalesPerOct; scl++) {
        vector<vector<float>> blurredImg =
            computeGussianBlur(currentImg, gKernelSize, sigma);
        octImages.push_back(blurredImg);
        sigma *= k;
      }
      scaleSpacePyramid.push_back(octImages);
      currentImg = downsampleImg(currentImg);
    }
    return scaleSpacePyramid;
  }

  vector<vector<float>> getInterScaleImgDiff(vector<vector<float>> &img1,
                                             vector<vector<float>> &img2) {
    int rows = img1.size();
    int cols = img1[0].size();
    assert(rows == img2.size() and cols == img2[0].size() &&
           "dimentions for image diff do not match");
    vector<vector<float>> diff(rows, vector<float>(cols, 0));
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        diff[i][j] = img1[i][j] - img2[i][j];
      }
    }
    return diff;
  }

  vector<vector<vector<vector<float>>>> computeDiffOfGaussianPyramid(
      vector<vector<vector<vector<float>>>> &ScaleSpacePyramid) {
    vector<vector<vector<vector<float>>>> diffOfGaussianPyrmd;
    assert(ScaleSpacePyramid.size() == numOctaves &&
           "SSP lenght does not match number of octaves");
    // For each octave, compute dog.
    for (int oct = 0; oct < numOctaves; oct++) {
      vector<vector<vector<float>>> diffOfGaussian;
      for (int scl = 1; scl < scalesPerOct; scl++) {
        // get the diff of scl-1 and scl.
        vector<vector<float>> diff = getInterScaleImgDiff(
            ScaleSpacePyramid[oct][scl - 1], ScaleSpacePyramid[oct][scl]);
        diffOfGaussian.push_back(diff);
      }
      diffOfGaussianPyrmd.push_back(diffOfGaussian);
    }
    return diffOfGaussianPyrmd;
  }

  // Get potential extrema for this scale.
  vector<pair<int, int>>
  searchInCurrentScale(int scale, vector<vector<vector<float>>> &Octave) {
    int OSize = Octave.size();
    assert(scale > 0 and scale < OSize - 1 &&
           "Required a Sandwich scale, Not a Sandwiched scale.");
    auto &lower = Octave[scale - 1];
    auto &middle = Octave[scale];
    auto &upper = Octave[scale + 1];
    vector<pair<int, int>> potentialKeypoints;
    int rows = middle.size();
    int cols = middle[0].size();
    for (int i = 1; i < rows - 1; i++) {
      for (int j = 1; j < cols - 1; j++) {
        float currPtVal = middle[i][j];
        float maxVal = FLT_MIN;
        float minVal = FLT_MAX;
        // first search current scale.
        for (int x = -1; x <= 1; x++) {
          for (int y = -1; y <= 1; y++) {
            // Skip the currentPtVal
            if (x == 0 and y == 0)
              continue;
            maxVal = max(maxVal, middle[i + x][j + y]);
            minVal = min(minVal, middle[i + x][j + y]);
          }
        }
        // Now search upper scale.
        for (int x = -1; x <= 1; x++) {
          for (int y = -1; y <= 1; y++) {
            maxVal = max(maxVal, upper[i + x][j + y]);
            minVal = min(minVal, upper[i + x][j + y]);
          }
        }
        // search lower scale.
        for (int x = -1; x <= 1; x++) {
          for (int y = -1; y <= 1; y++) {
            maxVal = max(maxVal, lower[i + x][j + y]);
            minVal = min(minVal, lower[i + x][j + y]);
          }
        }
        // current pixel is a candidate if it is greter than maximum or lesser
        // than minimum found.
        if (currPtVal > maxVal or currPtVal < minVal)
          potentialKeypoints.push_back(make_pair(i, j));
      }
    }
    return potentialKeypoints;
  }

  vector<vector<vector<pair<int, int>>>>
  findScaleSpaceExtrma(vector<vector<vector<vector<float>>>> &DoGPyramid) {
    assert(DoGPyramid.size() == numOctaves &&
           "DOG pyramid lenght does not match number of octaves");
    // AcrossOctaves, InAnOctave(Scales), Within a Scale
    vector<vector<vector<pair<int, int>>>> potentialKeyPoints;
    for (int oct = 0; oct < numOctaves; oct++) {
      // number of scales in this octave.
      int scs = DoGPyramid[oct].size();
      auto Octave = DoGPyramid[oct];
      vector<vector<pair<int, int>>> keyPointsInOctave;
      // Search this octaves to get points of interest, that is extrema points
      // wrt to 26, 3d points in scale above and below current scale.
      for (int sc = 1; sc < scs - 1; sc++) {
        // In this scale, search in scale above and below the extrema.
        auto keyPtsInScale = searchInCurrentScale(sc, Octave);
        keyPointsInOctave.push_back(keyPtsInScale);
      }
      potentialKeyPoints.push_back(keyPointsInOctave);
    }
    return potentialKeyPoints;
  }

public:
  SIFT() {
    // Scale factor Chosen to be /2 = 1.414
    k = 1.414;
    numOctaves = 4;
    scalesPerOct = 4;
    initalSigma = 1;
    gKernelSize = 3;
  }

  SIFT(float userK,unsigned userNumOct,unsigned userSclsPerOct,float userInitalSigma,unsigned userGaussianKernalSize){
    k = userK;
    numOctaves = userNumOct;
    scalesPerOct = userSclsPerOct;
    initalSigma = userInitalSigma;
    gKernelSize = userGaussianKernalSize;
  }

  void computeSIFT(Mat &image) {
    // get image data in float
    vector<vector<float>> img = getImageFltMatrix(image);
    vector<vector<vector<vector<float>>>> scaleSpacePyramid =
        computeScaleSpacePyramid(img);
#if DEBUG
    for (int i = 0; i < scaleSpacePyramid.size(); i++) {
      cout << "Octave = " << i;
      for (int j = 0; j < scaleSpacePyramid[i].size(); j++) {
        cout << " Scale = " << j;
        cout << " Img sizes = " << scaleSpacePyramid[i][j].size() << " x "
             << scaleSpacePyramid[i][j][0].size() << "\n";
      }
    }
#endif
    vector<vector<vector<vector<float>>>> DoGPyramid =
        computeDiffOfGaussianPyramid(scaleSpacePyramid);
    vector<vector<vector<pair<int, int>>>> scaleSpaceExtremas =
        findScaleSpaceExtrma(DoGPyramid);
    // this concludes inital scale space extrema detection step. Generates basic
    // keypoints which can be later pruned.
#if DEBUG
    unsigned count = 0;
    for (int oct = 0; oct < scaleSpaceExtremas.size(); oct++) {
      cout << ">>> Octave = " << oct;
      auto Octave = scaleSpaceExtremas[oct];
      cout << " Num Scales = " << Octave.size();
      for (int sc = 0; sc < Octave.size(); sc++) {
        auto keypts = Octave[sc];
        count += keypts.size();
        cout << " Keypts in scale = " << sc << " pts = " << keypts.size();
      }
      cout << "\n";
    }
#endif

  } // computeSIFT()
};

} // namespace MvSIFT

#endif