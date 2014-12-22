//
//  Brain.h
//  NetEvolution
//
//  Created by Christian on 12/21/14.
//  Copyright (c) 2014 Christian Cosgrove. All rights reserved.
//
#pragma once
#include "Neuron.h"
#include <vector>
class Brain
{
public:
    Brain();
    const int NUM_LAYERS = 3;
    const int HIDDEN_LAYER_WIDTH = 3;
    const int INPUT_LAYER_WIDTH = 2;
    const int OUTPUT_LAYER_WIDTH = 3;
    inline int GetNeuronIndex(int layerIndex, int index);
private:
    std::vector<Neuron> neurons;
};

inline int Brain::GetNeuronIndex(int layerIndex, int index)
{
    if (INPUT_LAYER_WIDTH + (layerIndex - 1) * HIDDEN_LAYER_WIDTH + index>=neurons.size()) throw std::out_of_range("Indexing out of range!");
    if (layerIndex==0)
        return index;
    else if (layerIndex<NUM_LAYERS-1)
        return INPUT_LAYER_WIDTH + (layerIndex - 1) * HIDDEN_LAYER_WIDTH + index;
}