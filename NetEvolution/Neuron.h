//
//  Neuron.h
//  NetEvolution
//
//  Created by Christian on 12/21/14.
//  Copyright (c) 2014 Christian Cosgrove. All rights reserved.
//
#pragma once
#include<vector>
#include <cmath>
class Neuron
{
public:
    struct NeuralConnection
    {
        size_t connection;
        float weight;
        inline float GetActivationFunction(std::vector<Neuron>& neurons)
        {
            return (0.5f*atan(neurons[connection].GetTransferFunction()) + 0.5f)*weight;
        }
    };
    
    Neuron();
    Neuron(const Neuron& copy);
    
    std::vector<NeuralConnection> connections;
    void Update(std::vector<Neuron>& neurons);
    
    void AddConnection(std::vector<Neuron>& neurons, size_t neuronIndex);
    
    inline void IncrementTransferFunction(float value);
    inline float GetTransferFunction();
    inline void ResetTransferFunction();
    inline void SetInputNeuron(float value);
private:
    float transferFunctionDefault = 0;
    float transferFunction = 0;
};

void Neuron::SetInputNeuron(float value) { transferFunctionDefault += value; }
void Neuron::IncrementTransferFunction(float value) { transferFunction+=value; }
float Neuron::GetTransferFunction() { return transferFunction; }
void Neuron::ResetTransferFunction() { transferFunction=transferFunctionDefault; }