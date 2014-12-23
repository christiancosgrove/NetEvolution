//
//  Neuron.cpp
//  NetEvolution
//
//  Created by Christian on 12/21/14.
//  Copyright (c) 2014 Christian Cosgrove. All rights reserved.
//

#include "Neuron.h"
#include "RandomUtils.h"

Neuron::Neuron()
{
    
}

Neuron::Neuron(const Neuron& copy) : connections(copy.connections), transferFunctionDefault(copy.transferFunctionDefault)
{

}


void Neuron::Update(std::vector<Neuron> &neurons)
{
    transferFunction = transferFunctionDefault;
    for (NeuralConnection& con : connections)
    {
        IncrementTransferFunction(con.GetActivationFunction(neurons));
    }
    
}

void Neuron::AddConnection(std::vector<Neuron>& neurons, size_t neuronIndex)
{
    if (neuronIndex > neurons.size()) throw std::out_of_range("Out of neuron array size");
    connections.push_back(NeuralConnection{neuronIndex, RandomUtils::Uniform<float>(-1,1)});
}