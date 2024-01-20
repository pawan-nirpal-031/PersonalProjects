#ifndef CORE
#define CORE

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

static Mat Convolution(Mat &img, vector<vector<double>> filter) {
  int FiltSize = filter.size();
  int trows = img.rows - FiltSize + 1;
  int tcols = img.cols - FiltSize + 1;
  Mat transformed(trows, tcols, CV_8U);
  for (int i = 1; i <= trows - 2; i++) {
    for (int j = 1; j <= tcols - 2; j++) {
      double tval = 0;
      for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
          tval = tval + (filter[x + 1][y + 1] *
                         static_cast<double>(img.at<u_char>(i + x, j + y)));
        }
      }
      transformed.at<u_char>(i, j) = static_cast<u_char>(min(255.0, tval));
    }
  }
  return (transformed);
}

// Fourier transform vairent fast fourier transform can perform convolution
// opreation of nxn image in n(logn) time this is the fastest method for
// convolution opreation. It is done using divide and conquer recursive
// algorithm. Implement convolution using fft. Similarlly corelation as well.
// vector<int> getValueRepresenation(vector<int> CoeffRep){
//   int n = CoeffRep.size();
//   if(n==1)
//     return CoeffRep;
//   double w = exp()
// }

// Impulse signal double flips on filter application. Convolution nullifies this
// double flip.  T(i,j)= $u$v (H(u,v)*I(i-u,j-u))  where $ is summation opreator
// T is the transformed image, H is the filter, and I is the input image. So
// convolution is cross correleation of double flipped input. This are linear
// shift invarient opreator, Linearity means Image I with filters h0 and h1 when
// applied are distributive ie, I(h0 + h1) = I*h0 + I*h1, also known as the
// super position principle. The other property is called shift invarience :
// shifting (or translating) a signal commutes with applying the opreator.
// T(i,j) = H(i+k,j+l) -> f(g(i,j)) = f(h(i+k,j+l)), In layman terms the effect
// of the opreator is the same no matter where in the image, ie it is invarient
// of the image location. Convolution (denoted by * opreator) is commutative a*b
// = b*a ( no diff btwn filter and signal/img) a*(b*c) = (a*b)*c helps in
// precomputations and reducing computation time, scalar : ka*b = a*kb =
// k*(a*b),Identity : Unit impulse. An important optimization uses property of
// seperablity usually for image of size nxn, and a filter size of k (usually 3)
// we require k^2 x n^2 computatations over the entire image, making it very
// expensive, seperablity allows us in some cases to bring this computation down
// to (2k * n^2) by performing 1d horizontal convolution followd by 1d vertial
// convolution having the same effect as the k^2 computations per pixel at end.
static void FastConvolution(Mat &img, vector<vector<double>> filter) {}

#endif