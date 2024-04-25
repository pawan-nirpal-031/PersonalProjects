/*
  Educational purposes only, Not be used in production code since SIFT is
  patented, "Method and apparatus for identifying scale invariant features in an
  image and use of same for locating an object in an image," David G. Lowe, US
  Patent 6,711,293 (March 23, 2004). Provisional application filed March 8,
  1999. Asignee: The University of British Columbia.

*/

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
vector<pair<int, int>> reject;

static void displayImage(Mat &img, unsigned int time = 0,
                         string title = "frame") {
  imshow(title, img);
  waitKey(time);
}


int main() {
  string imPath =
      "/home/panirpal/workspace/Projects/ComputerVision/data/images/astro1.jpg";
  Mat img = imread(imPath, IMREAD_GRAYSCALE);
  Mat cimg = imread(imPath);
  if (!img.empty()) {
    displayImage(img);
    
    displayImage(cimg);
  } else
    cerr << "image not found! exiting...";
}
