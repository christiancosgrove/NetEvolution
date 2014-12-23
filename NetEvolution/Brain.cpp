//
//  Brain.cpp
//  NetEvolution
//
//  Created by Christian on 12/21/14.
//  Copyright (c) 2014 Christian Cosgrove. All rights reserved.
//

#include "Brain.h"


Brain::Brain()
{
    for (int i = 0; i<INPUT_LAYER_WIDTH;i++)
    {
        neurons.push_back(Neuron());
    }
    for (int j=1;j<NUM_LAYERS-1;j++)
    {
        for (int i = 0; i<HIDDEN_LAYER_WIDTH;i++)
        {
            neurons.push_back(Neuron());
        }
    }
    for (int i = 0; i<OUTPUT_LAYER_WIDTH;i++)
    {
        neurons.push_back(Neuron());
    }
    for (int i = 0; i<INPUT_LAYER_WIDTH;i++)
    {
        for (int j=0;j<HIDDEN_LAYER_WIDTH;j++)
        {
            GetNeuron(1, j).AddConnection(neurons, GetNeuronIndex(0, i));
        }
    }
    for (int k = 1;k<NUM_LAYERS-1;k++)
    {
        for (int i=0;i<HIDDEN_LAYER_WIDTH;i++)
        {
            for (int j = 0; j<HIDDEN_LAYER_WIDTH;j++)
            {
                GetNeuron(k,j).AddConnection(neurons, GetNeuronIndex(k-1, i));
            }
        }
    }
    for (int i = 0; i<HIDDEN_LAYER_WIDTH;i++)
    {
        for (int j = 0; j<OUTPUT_LAYER_WIDTH;j++)
        {
            GetNeuron(NUM_LAYERS-1, j).AddConnection(neurons, GetNeuronIndex(NUM_LAYERS-1, i));
        }
    }
}

void Brain::Update()
{
    for (Neuron& neuron:neurons)
    {
        neuron.Update(neurons);
    }
}

void Brain::DuplicateBrain(const Brain &brain)
{
    if (neurons.size()!=brain.neurons.size()) throw std::out_of_range("Brain sizes do not match.");
    for (int i = 0; i<brain.neurons.size();i++)
    {
        neurons[i] = brain.neurons[i];
    }
        
}