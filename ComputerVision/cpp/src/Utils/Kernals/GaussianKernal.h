#ifndef GUASSIAN_KERNAL
#define GUASSIAN_KERNAL

#include "../Core/Core.h"

static double square(double x) { return x * x; }

static double computeGaussianFunc(double x, double y, double mu, double sigma) {
  double val =
      exp(-0.5 * ((square((x - mu) / sigma)) + square((y - mu) / sigma))) /
      (2 * M_PI * sigma * sigma);
  return val;
}

static vector<vector<double>> getGuassianKernal(double sigma) {
  int size = 2 * ceil(3 * sigma) + 1;
  vector<vector<double>> Kernal(size, vector<double>(size, 0.0));
  double sum = 0.0;
  int center = size / 2;
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      Kernal[i][j] = computeGaussianFunc(i, j, center, sigma);
      sum += Kernal[i][j];
    }
  }
  if (sum != 0) {
    for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
        Kernal[i][j] /= sum;
      }
    }
  }
  return Kernal;
}

#endif