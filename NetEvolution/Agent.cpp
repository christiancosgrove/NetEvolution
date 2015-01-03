//
//  Agent.cpp
//  NetEvolution
//
//  Created by Christian on 12/21/14.
//  Copyright (c) 2014 Christian Cosgrove. All rights reserved.
//

#include "Agent.h"
#include "glm/gtx/norm.hpp"
#include "RandomUtils.h"
const float Agent::MAX_HEALTH = 1000;

Agent::Agent(glm::vec3 position) : Position(position), Color(RandomUtils::UniformFloat(),RandomUtils::UniformFloat(),RandomUtils::UniformFloat())
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
    TimeAlive=0;
    Color = copy.Color;
    const float ColorMutationRate = 0.1f;
    Color.r += RandomUtils::Uniform<float>(-ColorMutationRate, ColorMutationRate);
    if (Color.r > 1) Color.r = 1;
    if (Color.r < 0) Color.r = 0;
    Color.g += RandomUtils::Uniform<float>(-ColorMutationRate, ColorMutationRate);
    if (Color.g > 1) Color.g = 1;
    if (Color.g < 0) Color.g = 0;
    Color.b += RandomUtils::Uniform<float>(-ColorMutationRate, ColorMutationRate);
    if (Color.b > 1) Color.b = 1;
    if (Color.b < 0) Color.b = 0;
}

void Agent::Reset()
{
    brain.Reset();
    Active=true;
    Health=MAX_HEALTH;
    TimeAlive=0;
    Color = glm::vec3(RandomUtils::UniformFloat(),RandomUtils::UniformFloat(),RandomUtils::UniformFloat());
}

void Agent::Update(std::vector<Plant>& plants, std::vector<Agent>& agents)
{
    Health-=1.f;
    if (Health<=0) { Active=false; }
    if (Active)
    {
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
            float yawDiff, pitchDiff;
            
            yawDiff = atan2(disp.z, disp.x);
            pitchDiff = atan2(sqrt(disp.x * disp.x + disp.z * disp.z), disp.y);
            
            
            if (nearDist<(nearestPlant->Radius+Radius)*(nearestPlant->Radius+Radius))
            {
                EatPlant(plants, index);
            }
            
            //set inputs
            brain.GetInputNeuron(0).SetInputNeuron(disp.length());
            //        brain.GetInputNeuron(1).SetInputNeuron(yawDiff);
            //        brain.GetInputNeuron(2).SetInputNeuron(pitchDiff);
            glm::vec3 dispNorm = glm::normalize(disp);
            brain.GetInputNeuron(1).SetInputNeuron(dispNorm.x);
            brain.GetInputNeuron(2).SetInputNeuron(dispNorm.y);
            brain.GetInputNeuron(3).SetInputNeuron(dispNorm.z);
        }
        {
            Agent* nearestAgent=nullptr;
            size_t index;
            float nearDist;
            for (int i = 0; i<agents.size();++i)
            {
                Agent& p = agents[i];
                float dist2 = glm::length2(Position - p.Position);
                if ((nearestAgent==nullptr || dist2 < nearDist) && &p!=this)
                {
                    nearestAgent = &p;
                    nearDist = dist2;
                    index=i;
                }
            }
            
            glm::vec3 disp = Position - nearestAgent->Position;
            //set inputs
            brain.GetInputNeuron(4).SetInputNeuron(disp.length());
            glm::vec3 dispNorm = glm::normalize(disp);
            brain.GetInputNeuron(5).SetInputNeuron(dispNorm.x);
            brain.GetInputNeuron(6).SetInputNeuron(dispNorm.y);
            brain.GetInputNeuron(7).SetInputNeuron(dispNorm.z);
            brain.GetInputNeuron(8).SetInputNeuron(glm::length(Color - nearestAgent->Color));
        }
        
        
        const float rotationRate = 0.01;
        //receive outputs
        brain.Update();
//        Yaw += (brain.GetOutputNeuron(0).GetTransferFunction())*rotationRate;
        
//        if (Yaw<-M_PI) Yaw += M_2_PI;
//        else if (Yaw>M_PI) Yaw -= M_2_PI;
//        Pitch += (brain.GetOutputNeuron(1).GetTransferFunction())*rotationRate;
//        if (Pitch<-M_PI_2) Pitch = -M_PI_2;
//        else if (Pitch>M_PI_2) Pitch = M_PI_2;
//        Roll += (brain.GetOutputNeuron(2).GetTransferFunction())*rotationRate;
//        if (Roll<-M_PI_2) Roll = -M_PI_2;
//        else if (Roll>M_PI_2) Roll = M_PI_2;
//        float speed = brain.GetOutputNeuron(3).GetTransferFunction();
////        printf("Y:%f,P:%f,R:%f\n", Yaw, Pitch, Roll);
//        Velocity = glm::vec3(glm::yawPitchRoll(glm::degrees(Yaw), glm::degrees(Pitch), glm::degrees(Roll)) * glm::vec4(1,0,0,1));
        Velocity = glm::vec3(brain.GetOutputNeuron(0).GetTransferFunction(), brain.GetOutputNeuron(1).GetTransferFunction(), brain.GetOutputNeuron(2).GetTransferFunction());
        Velocity = glm::normalize(Velocity);
        Position+=MaxSpeed*Velocity;//*getOrientation();
    TimeAlive++;
    }
}

void Agent::EatPlant(std::vector<Plant> &plants, size_t index)
{
    Health+=plants[index].Health;
    plants[index].Health=0;
    plants[index].Active=false;
}
