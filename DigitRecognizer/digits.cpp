#include "mathutils.h"
#include "neuralnet.h"

const int BATCH_SIZE = 20;
vector<Matrix> trainInput(42000), trainOutput(42000);

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
    Matrix input(1, 784), output(1, 10);
    trainDataStrem >> rowVal;
    vals = extractValsFromStr(rowVal);
    output.setZero();
    output.setValAt(0, vals[0], 1);
    for (int j = 1; j < 785; j++) {
      input.setValAt(0, j - 1, (vals[j] / 255.0));
    }

    trainInput[i] = input;
    trainOutput[i] = output;
  }
  #if DMODE
    cerr<<"Loaded data inp size "<<trainInput.size()<<" out size "<<trainOutput.size()<<"\n";
  #endif
  cerr << "Training data loaded.\n";
}

void randomShuffle(vector<int> &v) {
  for (int i = v.size() - 1; i >= 0; i--)
    swap(v[i], v[CoreMathUtils().nextInt() % (i + 1)]);
}

void trainingDriver(NeuralNet &model) {
  int epochs = 10;
  vector<int> trainAtRandomIdx(42000);
  for (int i = 0; i < 42000; i++)
    trainAtRandomIdx[i] = i;
  for (int e = 0; e < epochs; e++) {
    cerr << "Epoch : " << e << "\n";
    //randomShuffle(trainAtRandomIdx);
    for (int i = 0; i < 42000; i += BATCH_SIZE) {
      vector<Matrix> inputs(BATCH_SIZE), outputs(BATCH_SIZE);
      for (int j = 0; j < BATCH_SIZE; j++) {
        inputs[j] = (trainInput[trainAtRandomIdx[i + j]]);
        outputs[j] = (trainOutput[trainAtRandomIdx[i + j]]);
      }
      model.train(inputs, outputs);
    }
  }
}

int main() {
  parseTrainData();
  vector<unsigned> topology = {784, 20, 10};
  double learnRate = 1.0;
  NeuralNet digitRecg(topology, learnRate);
  trainingDriver(digitRecg);
}