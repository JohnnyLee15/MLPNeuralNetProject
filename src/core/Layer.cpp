#include "core/Layer.h"
#include "core/Neuron.h"
#include <cmath>
#include "utils/TrainingUtils.h"
#include "activations/Activation.h"

Layer::Layer(int numNeurons, int numWeights, Activation *activation):
    activations(numNeurons, 0), preActivations(numNeurons, 0.0), activation(activation) 
{
    for (int i = 0; i < numNeurons; i++) {
        neurons.push_back(Neuron(numWeights, activation));
    }
}

void Layer::calActivations(const vector<double> &prevActivations) {
    for (int i = 0; i < neurons.size(); i++) {
        preActivations[i] = neurons[i].calPreActivation(prevActivations);
    }

    activations = activation->activate(preActivations);
}

const vector<double>& Layer::getActivations() const {
    return activations;
}

const vector<double>& Layer::getPreActivations() const {
    return preActivations;
}

Activation* Layer::getActivation() const {
    return activation;
}

int Layer::getNumNeurons() const {
    return neurons.size();
}

void Layer::updateLayerParameters(
    const vector<double> &prevActivations,
    double learningRate,
    const vector<double> &outputGradient
) {
    for (int i = 0; i < neurons.size(); i++) {
        double deltaBias = outputGradient[i]*learningRate;
        neurons[i].updateBias(deltaBias);
        neurons[i].updateWeights(prevActivations, learningRate, outputGradient[i]);
    }
}

vector<double> Layer::updateOutputGradient(
    const vector<double> &prevOutputGradient,
    const vector<double> &prevPreActivations,
    Activation *prevActivation
) {
    vector<double> outputGradient(prevPreActivations.size(), 0.0);
    vector<double> activationGradient = prevActivation->calculateGradient(prevPreActivations);
    for (int j = 0; j < prevPreActivations.size(); j++) {
        outputGradient[j] = updateOutputDerivative(prevOutputGradient, j) * activationGradient[j];

    }
    return outputGradient;
}

double Layer::updateOutputDerivative(
    const vector<double> &prevOutputGradient,
    int weightIdx
) {

    double dz = 0;
    for (int i = 0; i < neurons.size(); i++) {
        dz += prevOutputGradient[i]*neurons[i].getWeight(weightIdx);
    }
    return TrainingUtils::clipDerivative(dz);
}

Layer::~Layer() {
    delete activation;
}
