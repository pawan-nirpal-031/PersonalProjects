#include "../ArtifactDetectors/Corners/HarrisCornerDetector.h"
#include "../ArtifactDetectors/Edges/CannyEdgeDetector.h"
#include "../ArtifactDetectors/FeatureDiscriptors/ScaleInvarientFeatureTransform.h"

int main() {
  string imPath =
      "/home/panirpal/workspace/Projects/ComputerVision/data/images/astro1.jpg";
  Mat img = imread(imPath, IMREAD_GRAYSCALE);
  if (!img.empty()) {
    displayImage(img);
    MvSIFT::SIFT().computeSIFT(img);
    // displayImage(img);
  } else
    cerr << "image not found! exiting...";
}