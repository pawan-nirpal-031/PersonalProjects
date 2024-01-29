#include <algorithm>
#include <math.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
using namespace std;
using namespace cv;

void displayImage(Mat &img, unsigned int time = 0, string title = "frame") {
  imshow(title, img);
  waitKey(time);
}

bool boundsCheck(int x,int y,int r,int c){
    return (x>=0 and x+1<r and y>=0 and y+1<c);
}

/*
766 551
383 275
*/

vector<vector<float>> downsampleImg(vector<vector<float>> currentImg){
  int rows = currentImg.size();
  int cols = currentImg[0].size();
  cout<<rows<<" "<<cols<<'\n';
  vector<vector<float>> output(rows/2,vector<float>(cols/2,0));
  cout<<output.size()<<" "<<output[0].size()<<"\n";
  for(int i = 0;i<rows;i+=2){
    for(int j = 0;j<cols;j+=2){
      if(not boundsCheck(i,j,rows,cols))
          continue;
      float val = 0;
      for(int v = 0;v<=1;v++){
          for(int w = 0;w<=1;w++){
              val+=currentImg[i+v][j+w];
          }
      }
      cout<<i/2<<" , "<<j/2<<"\n";
      // mean 
      output[i/2][j/2] = val/4;
    }
  }
  return output;
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

Mat getCvImage(vector<vector<float>> img){
    int rows = img.size();
    int cols = img[0].size();
    Mat out(rows,cols,CV_8U);
    for(int i =0;i<rows;i++){
        for(int j =0;j<cols;j++){
            out.at<uchar>(i,j) = static_cast<uchar>((int)img[i][j]);
        }
    }
    return out;
}

vector<vector<float>> downsampleImage2(vector<vector<float>> image) {
    int numRows = image.size();
    int numCols = image[0].size();
    int newNumRows = numRows / 2;
    int newNumCols = numCols / 2;
    vector<vector<float>> downsampledImage(newNumRows, vector<float>(newNumCols, 0.0));

    for (int i = 0; i < newNumRows; ++i) {
        for (int j = 0; j < newNumCols; ++j) {
            downsampledImage[i][j] = image[2 * i][2 * j];
        }
    }

    return downsampledImage;
}

int main() {
  string imPath =
      "/home/panirpal/workspace/Projects/ComputerVision/data/images/astro1.jpg";
  Mat img = imread(imPath, IMREAD_GRAYSCALE);
  if (!img.empty()) {
    displayImage(img);
    vector<vector<float>> i = getImageFltMatrix(img);
    vector<vector<float>> down = downsampleImg(i);
    Mat out = getCvImage(down);
    displayImage(out);
  } else
    cerr << "image not found! exiting...";
  return 0;
}
