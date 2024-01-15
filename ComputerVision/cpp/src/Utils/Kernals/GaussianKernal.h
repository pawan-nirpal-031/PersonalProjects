#ifndef GUASSIAN_KERNAL
#define GUASSIAN_KERNAL

#include "../Core/Core.h"

vector<vector<double>> getGuassianKernal(int size, double sigma) {
  vector<vector<double>> Kernal(size, vector<double>(size, 0.0));
  double sum = 0.0;
  int center = size / 2;
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      int x = i - center;
      int y = j - center;
      Kernal[i][j] = exp(-(x * x + y * y) / (2 * sigma * sigma));
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