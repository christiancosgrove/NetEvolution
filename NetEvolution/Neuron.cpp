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

void Neuron::Update(std::vector<Neuron> &neurons)
{
    for (NeuralConnection& con : connections)
    {
        neurons[con.connection].IncrementTransferFunction(con.GetActivationFunction(neurons));
    }
    
}

void Neuron::AddConnection(std::vector<Neuron>& neurons, size_t neuronIndex)
{
    connections.push_back(NeuralConnection{neuronIndex, RandomUtils::UniformFloat()});
}