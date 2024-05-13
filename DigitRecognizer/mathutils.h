#ifndef MATH_UTILS
#define MATH_UTILS

#include <assert.h>
#include <bits/stdc++.h>
using namespace std;

class CoreMathUtils {
  unsigned long x, y, z, w;

public:
  CoreMathUtils() {
    x = 52452667;
    y = 3524646277;
    z = 214253562674;
    w = 7986969l;
  }

  unsigned long nextInt() {
    unsigned long temp = x xor (x << 11);
    x = y;
    y = z;
    z = w;
    return w = (w xor (w >> 19) xor temp xor (temp >> 8));
  }

  double randZeroOne() { return (nextInt() % 1000000001) / 1000000001.00; }

  double sigmoid(double x) { return (1.0 / (1 + exp(-x))); }

  // x is the sigmoid value.
  double sigmoidDerivative(double x) { return x * (1 - x); }
};

class Matrix {
  int rows, cols;
  vector<vector<float>> data;
  CoreMathUtils g;

public:
  Matrix() { rows = cols = 0; }

  Matrix(int n, int m) {
    rows = n;
    cols = m;
    data.resize(n);
    for (int i = 0; i < n; i++)
      data[i].resize(m);
  }

  void printMatrix() {
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++)
        cout << data[i][j] << " ";
      cout << '\n';
    }
  }

  Matrix(const Matrix &given) {
    rows = given.rows;
    cols = given.cols;
    data.resize(rows);
    for (int i = 0; i < rows; i++)
      data[i].resize(cols);
    for (int i = 0; i < rows; i++)
      for (int j = 0; j < cols; j++)
        data[i][j] = given.data[i][j];
  }

  void copyMatrix(const Matrix &given) {
    rows = given.rows;
    cols = given.cols;
    data.resize(rows);
    for (int i = 0; i < rows; i++)
      data[i].resize(cols);

    for (int i = 0; i < rows; i++)
      for (int j = 0; j < cols; j++)
        data[i][j] = given.data[i][j];
  }

  inline unsigned getRows() const { return rows; }

  inline unsigned getCols() const { return cols; }

  pair<unsigned, unsigned> getDimensions() { return make_pair(rows, cols); }

  ~Matrix() {
    data.clear();
    rows = cols = 0;
  }

  void randomize() {
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        data[i][j] = g.randZeroOne();
        if (g.nextInt() % 2 == 0)
          data[i][j] = -data[i][j];
      }
    }
  }

  void setZero() {
    for (int i = 0; i < rows; i++)
      for (int j = 0; j < cols; j++)
        data[i][j] = 0.0;
  }

  void addToMatrix(Matrix &x) {
    assert(rows == x.rows and cols == x.cols and
           "non matching dimensions for accumulation");
    for (int i = 0; i < rows; i++)
      for (int j = 0; j < cols; j++)
        data[i][j] += x.data[i][j];
  }

  inline double getValAt(unsigned i, unsigned j) const { return data[i][j]; }

  inline void setValAt(unsigned i, unsigned j, double val) { data[i][j] = val; }
};

class MatrixOpreations {
public:
  MatrixOpreations() {}
  Matrix addMatrices(Matrix &x, Matrix &y) {
    assert(y.getRows() == x.getRows() and y.getCols() == x.getCols() and
           "non matching dimensions for addition");
    unsigned rows = x.getRows();
    unsigned cols = y.getCols();
    Matrix sum(rows, cols);
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++)
        sum.setValAt(i, j, x.getValAt(i, j) + y.getValAt(i, j));
    }
    return sum;
  }

  Matrix subtractMatrices(Matrix &x, Matrix &y) {
    assert(y.getRows() == x.getRows() and y.getCols() == x.getCols() and
           "non matching dimensions for subtraction");
    unsigned rows = x.getRows();
    unsigned cols = y.getCols();
    Matrix sum(rows, cols);
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++)
        sum.setValAt(i, j, x.getValAt(i, j) - y.getValAt(i, j));
    }
    return sum;
  }

  Matrix elementWiseProduct(Matrix &x, Matrix &y) {
    assert(y.getRows() == x.getRows() and y.getCols() == x.getCols() and
           "non matching dimensions for subtraction");
    unsigned rows = x.getRows();
    unsigned cols = y.getCols();
    Matrix prod(rows, cols);
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++)
        prod.setValAt(i, j, x.getValAt(i, j) * y.getValAt(i, j));
    }
    return prod;
  }

  void setAllValuesTo(Matrix &x, double val) {
    for (int i = 0; i < x.getRows(); i++) {
      for (int j = 0; j < x.getCols(); j++)
        x.setValAt(i, j, val);
    }
  }

  Matrix transpose(const Matrix &x) const {
    Matrix matT(x.getCols(), x.getRows());
    for (int i = 0; i < x.getRows(); i++) {
      for (int j = 0; j < x.getCols(); j++)
        matT.setValAt(j, i, x.getValAt(i, j));
    }
    return matT;
  }

  Matrix multiplyMatrices(const Matrix &x, const Matrix &y) {
    Matrix prod(x.getRows(), y.getCols());
    for (int i = 0; i < x.getRows(); i++) {
      for (int j = 0; j < y.getCols(); j++)
        for (int k = 0; k < x.getCols(); k++)
          prod.setValAt(
              i, j,
              (prod.getValAt(i, j) + (x.getValAt(i, k) * y.getValAt(k, j))));
    }
    return prod;
  }

  Matrix sigmoidMatrix(const Matrix &a) {
    Matrix sig(a.getRows(), a.getCols());
    for (int i = 0; i < a.getRows(); i++)
      for (int j = 0; j < a.getCols(); j++)
        sig.setValAt(i, j, CoreMathUtils().sigmoid(a.getValAt(i, j)));
    return sig;
  }

  Matrix sigmoidDerivativeMatrix(const Matrix &a) {
    Matrix sigD(a.getRows(), a.getCols());
    for (int i = 0; i < a.getRows(); i++)
      for (int j = 0; j < a.getCols(); j++)
        sigD.setValAt(i, j,
                      CoreMathUtils().sigmoidDerivative(a.getValAt(i, j)));
    return sigD;
  }
};

#endif
