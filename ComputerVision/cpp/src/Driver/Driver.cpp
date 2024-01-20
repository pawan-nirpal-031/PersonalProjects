#include "../ArtifactDetectors/Corners/HarrisCornerDetector.h"

int main() {
  string imPath =
      "/home/panirpal/workspace/Projects/ComputerVision/data/chess2.jpg";
  Mat img = imread(imPath, IMREAD_GRAYSCALE);
  if (!img.empty()) {
    Mat gaussian = guassianFilterTransform(img);
    Mat out = detectHarrisCorners(gaussian);
    displayImage(out);
  } else
    cerr << "image not found! exiting...";
}