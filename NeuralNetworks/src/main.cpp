#include "../include/Matrix.h"
#include "../include/NeuralNetwork.h"
#include "../include/Neuron.h"
#include <chrono>
#include <iomanip>
#include <fstream>
#define PerfMode 0
#define DRIVER_AVT 1
using namespace std;
using namespace chrono;

const int BATCH_SIZE = 20;
vector<Matrix*> trainInput, trainOutput;

vector<int> extractValsFromStr(string vals) {
  vector<int> data;
  int currInt = 0;
  for (int i = 0; i < vals.length(); i++) {
    if (vals[i] == ',') {
      data.push_back(currInt);
      currInt = 0;
    } else {
      currInt *= 10;
      currInt += vals[i] - '0';
    }
  }
  data.push_back(currInt);
  return data;
}

void parseTrainData() {
  ifstream trainDataStrem("train.csv");
  string rowVal;
  trainDataStrem >> rowVal;
  vector<int> vals;

  for (int i = 0; i < 42000; i++) {
    Matrix* input = new Matrix(1, 784,0);
    Matrix* output = new Matrix (1, 10,0);
    trainDataStrem >> rowVal;
    vals = extractValsFromStr(rowVal);
    output->setZero();
    output->setValue(0, vals[0], 1);
    for (int j = 1; j < 785; j++) {
      input->setValue(0, j - 1, (vals[j] / 255.0));
    }
    trainInput.push_back(input);
    trainOutput.push_back(output);
  }
#if DRIVER_AVT
  cerr << "Loaded data inp size " << trainInput.size() << " out size "
       << trainOutput.size() << "\n";
#endif
}

void randomShuffle(vector<int> &v) {
  CoreMathUtils mobj;
  for (int i = v.size() - 1; i >= 0; i--)
    swap(v[i], v[mobj.nextInt() % (i + 1)]);
}

void trainingDriver(NeuralNetwork &model) {
  int epochs = 10;
  float error;
  vector<int> trainAtRandomIdx(42000);
  for (int i = 0; i < 42000; i++)
    trainAtRandomIdx[i] = i;
  for (int e = 0; e < epochs; e++) {
#if DRIVER_AVT
    cerr << "Epoch : " << e << "\n";
#endif
    //randomShuffle(trainAtRandomIdx);
    for (int i = 0; i < 42000; i += BATCH_SIZE) {
      vector<Matrix*> inputs(BATCH_SIZE), outputs(BATCH_SIZE);
      for (int j = 0; j < BATCH_SIZE; j++) {
        inputs[j] = (trainInput[trainAtRandomIdx[i + j]]);
        outputs[j] = (trainOutput[trainAtRandomIdx[i + j]]);
      }
      model.train(inputs, outputs);
    }

    // for (int i = 0; i < 42000; i++) {
    //   Matrix CurrentOutput = model.feedForward(trainInput[i]);
    //   for (int j = 0; j < 10; j++)
    //     error += CoreMathUtils::square(CurrentOutput.getValAt(0, j) -
    //                                    trainOutput[i].getValAt(0, j));
    // }

    // error /= 10.0;
    // error /= 42000.0;

#if DRIVER_AVT
    cerr << "Error : " << error << " Epoch End\n";
#endif
  }
}

int main(int Argc, char **Argv) {
#if PerfMode
  auto start = high_resolution_clock::now();
#endif
  parseTrainData();
  vector<int> topology = {784, 20, 10};
  NeuralNetwork nn(topology);
  trainingDriver(nn);
#if PerfMode
  auto stop = high_resolution_clock::now();
  double time_taken = duration_cast<nanoseconds>(stop - start).count();
  cout << fixed << time_taken << setprecision(9) << " ";
#endif
  return 0;
}