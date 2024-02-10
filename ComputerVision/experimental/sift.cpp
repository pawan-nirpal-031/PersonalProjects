#include "assert.h"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#define DEBUG 0
#define IMGLVLDIAGNOSIS 0
#define KEYPOINTPLT 0
using namespace std;
using namespace cv;

// Use subPatchSize x subPatchSize image in the orignal image to obtain
// debugging info.
const unsigned subPatchSize = 6;

static void displayImage(Mat &img, unsigned int time = 0,
                         string title = "frame") {
  imshow(title, img);
  waitKey(time);
}

static float square(float x) { return x * x; }

static float computeGaussianFunc(float x, float y, float mu, float sigma) {
  float val =
      exp(-0.5 * ((square((x - mu) / sigma)) + square((y - mu) / sigma))) /
      (2 * M_PI * sigma * sigma);
  return val;
}

bool isImgPoint(int rows, int cols, int x, int y) {
  return (x >= 0 and x < rows and y >= 0 and y < cols);
}

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

vector<vector<float>> computeGussianBlur(vector<vector<float>> &img, float s) {
  int Ksize = 2 * ceil(3 * s) + 1;
  vector<vector<float>> res;
  pair<vector<vector<float>>, float> KernalInfo;
  KernalInfo = computeKernalInfo(Ksize, s);
  vector<vector<float>> gaussianFilterd =
      computeGaussianConvolution(KernalInfo, Ksize, img);
  return gaussianFilterd;
}

vector<vector<float>> getImageFltMatrix(Mat &img) {
  int rows = img.rows;
  int cols = img.cols;
  vector<vector<float>> grayScalImg(rows, vector<float>(cols, 0));
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
  return grayScalImg;
}

void plotImageMatrix(vector<vector<float>> &img) {
  int rows = img.size();
  int cols = img[0].size();
  Mat image(rows, cols, CV_8U);
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      image.at<u_char>(i, j) = static_cast<u_char>((int)(img[i][j]));
    }
  }
  displayImage(image);
}

void plotKeyPointsInImg(Mat &img, vector<pair<int, int>> &points) {
  for (auto point : points) {
    circle(img, Point(point.second, point.first), 3, Scalar(0, 255, 0), 1);
  }
}

void logSubPatchPrint(const vector<vector<float>> &img) {
  if (img.size() > 9 and img[0].size() > 9) {
    cout << "START : ====================\n\n";
    for (int i = 1; i <= subPatchSize; i++) {
      for (int j = 1; j <= subPatchSize; j++) {
        cout << img[i][j] << " ";
      }
      cout << "\n";
    }
    cout << "\nEND : ====================\n\n";
  }
}

namespace MvSIFT {

class SIFT {
  float k;
  int numOctaves;
  int scalesPerOct;
  float initalSigma;
  int gKernelSize;
  unsigned refinementIters;

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
  computeScaleSpacePyramid(vector<vector<float>> &img) {
    vector<vector<vector<vector<float>>>> scaleSpacePyramid;
    vector<vector<float>> currentImg = img;
    auto propTonxtOct = img;
    for (int oct = 1; oct <= numOctaves; oct++) {
      vector<vector<vector<float>>> octImages;
      float sigma = initalSigma;
      for (int scl = 1; scl <= scalesPerOct; scl++) {
        vector<vector<float>> blurredImg =
            computeGussianBlur(currentImg, sigma);
        octImages.push_back(blurredImg);
        currentImg = blurredImg;
        sigma *= k;
      }
      scaleSpacePyramid.push_back(octImages);
      currentImg = downsampleImg(propTonxtOct);
      propTonxtOct = currentImg;
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
            ScaleSpacePyramid[oct][scl], ScaleSpacePyramid[oct][scl - 1]);
        diffOfGaussian.push_back(diff);
      }
      diffOfGaussianPyrmd.push_back(diffOfGaussian);
    }
    return diffOfGaussianPyrmd;
  }

  bool toRefineOrDiscardPotentialKeyPoint(unsigned x,unsigned y,unsigned scale,vector<vector<vector<float>>> &Octave){
    bool isKeyPtValid = false;
    for(unsigned iter = 0;iter<refinementIters;iter++){
      
    }
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
        if (currPtVal > maxVal or currPtVal < minVal) {
          // potentialKeypoints.push_back(make_pair(i, j));
          if(toRefineOrDiscardPotentialKeyPoint(i,j,scale,Octave))
            //potentialKeypoints.push_back(make_pair(i, j));
        }
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
    scalesPerOct = 5;
    initalSigma = 1;
    gKernelSize = 3;
    refinementIters = 3;
  }

  SIFT(float userK, unsigned userNumOct, unsigned userSclsPerOct,
       float userInitalSigma, unsigned userGaussianKernalSize, unsigned refineIters) {
    k = userK;
    numOctaves = userNumOct;
    scalesPerOct = userSclsPerOct;
    initalSigma = userInitalSigma;
    gKernelSize = userGaussianKernalSize;
    refinementIters = refineIters;
  }
  /*
  Implmn Details:
      1) Converting Mat -> vector<vector<float>> and only dealing with image as
  a matrix floats, since static_casting back and fourth from u_char costs in
  data loss.

      2) A pyramid is a vector<Octaves> an Octave is a vector<Scales> a scale is
  an image level rep/ vector<vector<float>>

      So the
            img/scale = vector<vector<float>>
            octave = vector<vector<vector<float>>> or vector<scale>
            pyramid = vector<vector<vector<vector<float>>>> or vector<octave>

      3) Keypoints in a scale are vector<pair<int,int>>, a vector of
  co-ordinates.

  Params : (as set in default constructor)
      k = 1.414 (Scale sigma multiplier) : as suggested in sift paper

      num of octaves = 4

      num of scales per octave = 4

      gaussian kernel size = 3 : This could be a potential source of bugs, as
  this has to be ideally computed based on given sigma.

  Algorithm (Partially completed yet):

  https://www.analyticsvidhya.com/blog/2019/10/detailed-guide-powerful-sift-technique-image-matching-python/

  Following the Std algorithm as per my level of understanding so far.
      1) Compute Scale space pyramid
          Get a scale space pyramid for each octave (4 octaves), compute 4
  scales ( number of scales per octave = 4). Start with inital img, and compute
  Gaussian blurred image with initalSigma, and for the successive scales in this
  octave, compute Gaussian blur of previously blurred image with a sigma of =
  previousSigma * k, this achieves successive blurring in an octave. So we have
  4 scales in an octave. Now downsample the orignal image ( for octave = 1,
  octaves = (0 1 2 3)) get the image of half the dimensions of source image, and
  perform the above steps, and for each successive octave downsample the image
  of previous octave. Finally return the scaleSpacePyramid.

      2) Compute Difference of Gaussian Pyramid.
          Get a pyramid of Differences of Gaussians previously computed, For
  each octave, traverse the scale from (1 , 2 , 3), and compute the diff of
  scale(i) - scale(i-1). So we will be left with 3 scales in DoGPyramid. To
  compute diff of scale(i) - scale(i-1) for i = 1,2,3. Take element wise diff of
  image in scale i and image in scale i-1. Finally return this Pyramid.

      3) Compute Scale Space Extremas.
          Using The DoGPyramid For each octave (4 octaves yet),
  there are 3 scales per octave in DogPyramid (0,1,2). Now to
  generalize, say there are s scales per octave in DoGPyramid
  (0,1,2 ... s-1, s), for scales [1,s-1], for a scale s for
  every pixel in [1,rows-1][1,cols-1] range, there are 8 pixels
  around it in the current scale, and 9 pixel in the scale s+1,
  and 9 in the scale s-1. Compute the maximum in the rest of
  the 26 pixels (8 in scale s, 9 in scale s+1, 9 in scale s-1),
  I name scale s as middle, scale s+1 as upper and scale s-1 as lower. And if
  this current point value is higher or equal than the maximum, lower or equal
  than the minimum, then mark it as a potential keypoint and insert it in a list
  of points, and return it.

  */
  void computeSIFT(Mat &image, Mat &ClrCpy) {
    // get image data in float
    vector<vector<float>> img = getImageFltMatrix(image);
    vector<vector<vector<vector<float>>>> scaleSpacePyramid =
        computeScaleSpacePyramid(img);
#if DEBUG
    for (int i = 0; i < scaleSpacePyramid.size(); i++) {
      cout << "Octave = " << i << "\n";
      for (int j = 0; j < scaleSpacePyramid[i].size(); j++) {
        cout << " Scale = " << j;
        cout << " Img sizes = " << scaleSpacePyramid[i][j].size() << " x "
             << scaleSpacePyramid[i][j][0].size() << "\n";
        // plotImageMatrix(scaleSpacePyramid[i][j]);
        // logSubPatchPrint(scaleSpacePyramid[i][j]);
      }
    }
    cout << "\n";
#endif
    vector<vector<vector<vector<float>>>> DoGPyramid =
        computeDiffOfGaussianPyramid(scaleSpacePyramid);
#if DEBUG
    for (int oct = 0; oct < DoGPyramid.size(); oct++) {
      auto Octave = DoGPyramid[oct];
      cout << ">>> Diff Of Gaussian Octave = " << oct;
      cout << " Num Scales = " << Octave.size() << "\n ";
      for (int scl = 0; scl < Octave.size(); scl++) {
        auto dogImg = Octave[scl];
        plotImageMatrix(dogImg);
        logSubPatchPrint(dogImg);
#if IMGLVLDIAGNOSIS
        for (int i = 0; i < dogImg.size(); i++) {
          for (int j = 0; j < dogImg[0].size(); j++) {
            cout << dogImg[i][j] << " ";
          }
          cout << "\n";
        }
        cout << "\n\n";
#endif
      }
    }
#endif

    vector<vector<vector<pair<int, int>>>> scaleSpaceExtremas =
        findScaleSpaceExtrma(DoGPyramid);
    // this concludes inital scale space extrema detection step. Generates basic
    // keypoints which can be later pruned.
#if DEBUG
    unsigned count = 0;
    for (int oct = 0; oct < scaleSpaceExtremas.size(); oct++) {
      cout << ">>> Scale Space Extrema Octave = " << oct << "\n";
      auto Octave = scaleSpaceExtremas[oct];
      cout << " Num Scales = " << Octave.size();
      for (int sc = 0; sc < Octave.size(); sc++) {
        auto keypts = Octave[sc];
        count += keypts.size();
        cout << " Keypts in scale" << sc << " Keypts = " << keypts.size();
      }
      cout << "\n";
    }
#endif

#if !KEYPOINTPLT
    for (auto &Octave : scaleSpaceExtremas) {
      for (auto &scale : Octave) {
        plotKeyPointsInImg(ClrCpy, scale);
      }
    }
#endif
    
  } // computeSIFT()
};

} // namespace MvSIFT

int main() {
  string imPath =
      "/home/panirpal/workspace/Projects/ComputerVision/data/images/astro1.jpg";
  Mat img = imread(imPath, IMREAD_GRAYSCALE);
  Mat cimg = imread(imPath);
  if (!img.empty()) {
    displayImage(img);
    MvSIFT::SIFT().computeSIFT(img, cimg);
    displayImage(cimg);
  } else
    cerr << "image not found! exiting...";
}
