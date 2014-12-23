//
//  Agent.cpp
//  NetEvolution
//
//  Created by Christian on 12/21/14.
//  Copyright (c) 2014 Christian Cosgrove. All rights reserved.
//

#include "Agent.h"
#include "glm/gtx/euler_angles.hpp"
#include "glm/gtx/norm.hpp"

const float Agent::MAX_HEALTH = 100;

Agent::Agent(glm::vec3 position) : Position(position)
{
    
}

void Agent::Update(std::vector<Plant>& plants)
{
    Plant* nearestPlant=nullptr;
    float nearDist;
    for (Plant& p:plants)
    {
        float dist2 = glm::length2(Position - p.Position);
        if (nearestPlant==nullptr || dist2 < nearDist)
        {
            nearestPlant = &p;
            nearDist = dist2;
        }
    }
    
    //set inputs
    brain.GetInputNeuron(0).SetInputNeuron(nearDist);
    brain.GetInputNeuron(1).SetInputNeuron(1);
    
    //receive outputs
    brain.Update();
    Yaw = brain.GetOutputNeuron(0).GetTransferFunction()/10;
    Pitch = brain.GetOutputNeuron(1).GetTransferFunction()/10;
    Roll = brain.GetOutputNeuron(2).GetTransferFunction()/10;
//    printf("Y:%f,P:%f,R:%f\n", Yaw, Pitch, Roll);
    Velocity = glm::vec3(glm::yawPitchRoll(Yaw, Pitch, Roll) * glm::vec4(Velocity,1));
    Position+=MaxSpeed*Velocity;
}

