#ifndef MATRIX_H 
#define MATRIX_H 
#include <vector>
#include <math.h>
using namespace std; 

class Matrix{
private: 
    int rows; 
    int cols;
    vector<vector<double>>Values;
public: 
    Matrix(int rows,int cols,bool isRandom);
    Matrix* transpose(); 
    void setValue(int row,int col,double Value);
    double getValue(int row,int col) const;
    double generateRandomNumber();
    int getNumRows() const { return rows;}
    int getNumCols() const { return cols;}
    void printToConsole();
    void setZero();
};

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

  static double sigmoid(double x) { return (1.0 / (1 + exp(-x))); }

  // x is the sigmoid value.
  static double sigmoidDerivative(double x) { return x * (1 - x); }

  static double square(double x) { return x * x; }
};

#endif