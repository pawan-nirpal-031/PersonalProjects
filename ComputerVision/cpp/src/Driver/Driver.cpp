#include "../ArtifactDetectors/Corners/HarrisCornerDetector.h"
#include "../ArtifactDetectors/Edges/CannyEdgeDetector.h"

int main() {
  string imPath =
      "/home/panirpal/workspace/Projects/ComputerVision/data/images/draw.jpg";
  Mat img = imread(imPath);
  if (!img.empty()) {
    displayImage(img);
    HarrisCornerDetector().detectHarrisCorners(img);
    displayImage(img);
  } else
    cerr << "image not found! exiting...";
}