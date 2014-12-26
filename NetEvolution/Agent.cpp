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
#include "RandomUtils.h"
const float Agent::MAX_HEALTH = 1000;

Agent::Agent(glm::vec3 position) : Position(position)
{
    
}

void Agent::Duplicate(const Agent &copy)
{
    brain.DuplicateBrain(copy.brain);
    Position = copy.Position;
    Yaw = copy.Yaw;
    Pitch = copy.Pitch;
    Roll = copy.Roll;
    Active=true;
    Health=MAX_HEALTH;
}

void Agent::Reset()
{
    brain.Reset();
    Active=true;
    Health=MAX_HEALTH;
}

void Agent::Update(std::vector<Plant>& plants)
{
    Health-=0.5f;
    if (Health<=0) Active=false;
    if (Active)
    {
        Plant* nearestPlant=nullptr;
        size_t index;
        float nearDist;
        for (int i = 0; i<plants.size();++i)
        {
            Plant& p = plants[i];
            float dist2 = glm::length2(Position - p.Position);
            if (nearestPlant==nullptr || dist2 < nearDist)
            {
                nearestPlant = &p;
                nearDist = dist2;
                index=i;
            }
        }
        
        glm::vec3 disp = Position - nearestPlant->Position;
//        float yawDiff, pitchDiff;
        
        glm::vec3 up(glm::normalize(glm::cross(Velocity, glm::vec3(0,0,1))));
        glm::vec3 side(glm::normalize(glm::cross(Velocity,up)));
        float diff1 = acos(glm::dot(up,disp) / (glm::length(disp)));
        float diff2 = acos(glm::dot(side,disp) / (glm::length(disp)));
//        yawDiff = atan2(disp.z, disp.x);
//        pitchDiff = atan2(sqrt(disp.x * disp.x + disp.z * disp.z), disp.y);
        
        
        if (nearDist<(nearestPlant->Radius+Radius)*(nearestPlant->Radius+Radius))
        {
            EatPlant(plants, index);
        }
        
        //set inputs
        brain.GetInputNeuron(0).SetInputNeuron(nearDist);
        brain.GetInputNeuron(1).SetInputNeuron(diff1);
        brain.GetInputNeuron(2).SetInputNeuron(diff2);
        
        
        
        //receive outputs
        brain.Update();
        Yaw = (brain.GetOutputNeuron(0).GetTransferFunction())/2;
        Pitch = (brain.GetOutputNeuron(1).GetTransferFunction())/2;
//        Roll = (brain.GetOutputNeuron(2).GetTransferFunction())/50;
        float speed = brain.GetOutputNeuron(2).GetTransferFunction();
        Roll=0;
    //    printf("Y:%f,P:%f,R:%f\n", Yaw, Pitch, Roll);
        Velocity = glm::vec3(glm::yawPitchRoll(Yaw, Pitch, Roll) * glm::vec4(Velocity,1));
        Position+=MaxSpeed*speed*Velocity;
    }
}

void Agent::EatPlant(std::vector<Plant> &plants, size_t index)
{
    Health+=plants[index].Health;
    plants[index].Health=0;
    plants[index].Active=false;
}
