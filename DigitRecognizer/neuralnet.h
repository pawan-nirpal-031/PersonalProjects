#ifndef NEURAL_NET
#define NEURAL_NET

#include "mathutils.h"

class NeuralNet {
  unsigned numLayers;
  vector<unsigned> laySizes;
  vector<Matrix> weights, bais, deltaWeights, deltaBais;
  double learningRate;

  Matrix computeActivationFunction(Matrix &weight, Matrix &bais, Matrix input) {
    MatrixOpreations matOps;
    Matrix product = matOps.multiplyMatrices(input, weight);
    Matrix baisSum = matOps.addMatrices(product, bais);
    Matrix activation = matOps.sigmoidMatrix(baisSum);
    return activation;
  }

public:
  NeuralNet() {}
  NeuralNet(vector<unsigned> &laySz, double userLearnRate) {
    numLayers = laySz.size();
    laySizes = laySz;
    learningRate = userLearnRate;
    weights.resize(numLayers - 1);
    bais.resize(numLayers - 1);
    deltaWeights.resize(numLayers - 1);
    deltaBais.resize(numLayers - 1);

    for (int i = 0; i < numLayers - 1; i++) {
      weights[i] = Matrix(laySizes[i], laySizes[i + 1]);
      bais[i] = Matrix(1, laySizes[i + 1]);
      deltaWeights[i] = Matrix(laySizes[i], laySizes[i + 1]);
      deltaBais[i] = Matrix(1, laySizes[i + 1]);
      weights[i].randomize();
      bais[i].randomize();
    }
  }

  Matrix feedForward(Matrix input) {
    MatrixOpreations matOps;
    for (int i = 0; i < numLayers - 1; i++)
      input = computeActivationFunction(weights[i], bais[i], input);
    return input;
  }

  void backpropagation(Matrix input, Matrix output) {
    vector<Matrix> layers;
    layers.push_back(input);
    MatrixOpreations matOps;
    Matrix delta;
    for (int i = 0; i < numLayers - 1; i++) {
      input = computeActivationFunction(weights[i], bais[i], input);
      layers.push_back(input);
    }
    // Cost function parital derivative. d(a - y)^2 = 2(a - y) ~ (a - y). a =
    // final layer output, y = ground truth
    delta = matOps.subtractMatrices(input, output);
    Matrix sigmodD = matOps.sigmoidDerivativeMatrix(layers[numLayers - 1]);
    // Cost function parital derivative wrt to baises
    delta = matOps.elementWiseProduct(delta, sigmodD);
    // accumulate dBais, dWeights (gradients)
    deltaBais[numLayers - 2].addToMatrix(delta);

    Matrix SecondLastT = matOps.transpose(layers[numLayers - 2]);
    Matrix dWAccum = matOps.multiplyMatrices(SecondLastT, delta);
    deltaWeights[numLayers - 2].addToMatrix(dWAccum);

    for (int i = numLayers - 3; i >= 0; i--) {
      Matrix t1 = matOps.transpose(weights[i + 1]);
      delta = matOps.multiplyMatrices(delta, t1);
      sigmodD = matOps.sigmoidDerivativeMatrix(layers[i + 1]);
      delta = matOps.elementWiseProduct(delta, sigmodD);
      deltaBais[i].addToMatrix(delta);
      t1 = matOps.transpose(layers[i]);
      Matrix prodTransDelta = matOps.multiplyMatrices(t1, delta);
      deltaWeights[i].addToMatrix(prodTransDelta);
    }
  }

  void updateWeightsAtIdx(int idx, int x, int y) {
    double diffTerm = learningRate * deltaWeights[idx].getValAt(x, y);
    weights[idx].setValAt(x, y, (weights[idx].getValAt(x, y) - diffTerm));
  }

  void updateBaisAtIdx(int idx, int x, int y) {
    double diffTerm = learningRate * deltaBais[idx].getValAt(x, y);
    bais[idx].setValAt(x, y, (bais[idx].getValAt(x, y) - diffTerm));
  }

  void train(vector<Matrix> &inputs, vector<Matrix> &outputs) {
#if DMODE
    cerr << "training started...\n";
#endif
    for (int i = 0; i < numLayers - 1; i++) {
      deltaWeights[i].setZero();
      deltaBais[i].setZero();
    }

    for (int i = 0; i < inputs.size(); i++) {
      Matrix input(inputs[i]);
      Matrix output(outputs[i]);
#if DMODE
      cerr << " Inp size : " << input.getRows() << " x " << input.getCols()
           << " | Output Size : " << output.getRows() << " x "
           << output.getCols() << "\n";
#endif
      backpropagation(input, output);
    }
    return;

    for (int i = 0; i < numLayers - 1; i++) {
      // update weights
      for (int j = 0; j < deltaWeights[i].getRows(); j++) {
        for (int k = 0; k < deltaWeights[i].getCols(); k++) {
          deltaWeights[i].setValAt(
              j, k, (deltaWeights[i].getValAt(j, k) / double(inputs.size())));
          updateWeightsAtIdx(i, j, k);
        }
      }

      // update baises
      for (int j = 0; j < deltaBais[i].getRows(); j++) {
        for (int k = 0; k < deltaBais[i].getCols(); k++) {
          deltaBais[i].setValAt(
              j, k, (deltaBais[i].getValAt(j, k) / double(inputs.size())));
          updateBaisAtIdx(i, j, k);
        }
      }
    }
  }
};

#endif