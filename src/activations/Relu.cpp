#include "activations/Relu.h"
#include <algorithm>

const double Relu::RELU_BIAS = 0.01;

vector<double> Relu::activate(const vector<double>& z) const{
    vector<double> activations(z.size(), 0.0);
    for (int i = 0; i < z.size(); i++) {
        activations[i] = max(0.0, z[i]);
    }
    return activations;
}

double Relu::initBias() const {
    return RELU_BIAS;
}

vector<double> Relu::calculateGradient(const vector<double> &preActivations) const {
    vector<double> gradient(preActivations.size(), 0.0);
    for (int i = 0; i < preActivations.size(); i++) {
        if (preActivations[i] > 0) {
            gradient[i] = 1;
        } else {
            gradient[i] = 0;
        }
    }
    return gradient;
}