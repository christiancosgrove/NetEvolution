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
            return atan(neurons[connection].GetTransferFunction());
        }
    };
    
    Neuron();
    
    std::vector<NeuralConnection> connections;
    void Update(std::vector<Neuron>& neurons);
    
    void AddConnection(std::vector<Neuron>& neurons, size_t neuronIndex);
    
    inline void IncrementTransferFunction(float value);
    inline float GetTransferFunction();
private:
    float transferFunction;
};

void Neuron::IncrementTransferFunction(float value) { transferFunction+=value; }
float Neuron::GetTransferFunction() { return transferFunction; }