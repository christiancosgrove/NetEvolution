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
    
    void DuplicateBrain(const Brain& brain);
    const int NUM_LAYERS = 3;
    const int HIDDEN_LAYER_WIDTH = 2;
    const int INPUT_LAYER_WIDTH = 3;
    const int OUTPUT_LAYER_WIDTH = 3;
    void Reset();
    inline int NUM_NEURONS() const;
    inline int GetNeuronIndex(int layerIndex, int index) const;
    inline Neuron& GetNeuron(int layerIndex, int index);
    inline Neuron& GetOutputNeuron(int index);
    inline Neuron& GetInputNeuron(int index);
    void Update();
    void Mutate();
private:
    std::vector<Neuron> neurons;
};
int Brain::NUM_NEURONS() const
{
    return NUM_LAYERS*HIDDEN_LAYER_WIDTH + INPUT_LAYER_WIDTH+OUTPUT_LAYER_WIDTH;
}

int Brain::GetNeuronIndex(int layerIndex, int index) const
{
    if (INPUT_LAYER_WIDTH + std::max((layerIndex - 1) * HIDDEN_LAYER_WIDTH,0) + index>=neurons.size()) throw std::out_of_range("Indexing out of range!");
    if (layerIndex==0)
        return index;
    else 
        return INPUT_LAYER_WIDTH + (layerIndex - 1) * HIDDEN_LAYER_WIDTH + index;
}

Neuron& Brain::GetNeuron(int layerIndex, int index)
{
    return neurons[GetNeuronIndex(layerIndex, index)];
}

Neuron& Brain::GetOutputNeuron(int index)
{
    return GetNeuron(NUM_LAYERS-1, index);
}
Neuron& Brain::GetInputNeuron(int index)
{
    return GetNeuron(0, index);
}