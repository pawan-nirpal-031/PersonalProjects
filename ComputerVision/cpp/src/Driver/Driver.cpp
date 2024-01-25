#include "../ArtifactDetectors/Corners/HarrisCornerDetector.h"
#include "../ArtifactDetectors/Edges/CannyEdgeDetector.h"

int main() {
  string imPath =
      "/home/panirpal/workspace/Projects/ComputerVision/data/images/chess2.jpg";
  Mat img = imread(imPath, IMREAD_GRAYSCALE);
  if (!img.empty()) {
    displayImage(img);
    Mat gaussian = guassianFilterTransform(img, 1.5);
    // Mat out = detectHarrisCornersUpd(gaussian);
    displayImage(gaussian);
  } else
    cerr << "image not found! exiting...";
}