
//
//  Agent.h
//  NetEvolution
//
//  Created by Christian on 12/21/14.
//  Copyright (c) 2014 Christian Cosgrove. All rights reserved.
//
#pragma once
#include "glm/glm.hpp"
#include "Brain.h"
#include "AgentRenderNode.h"
#include "Plant.h"
#include "glm/gtx/euler_angles.hpp"

class Agent
{
public:
    Agent(glm::vec3 position);
    void Duplicate(const Agent& copy);
    void Reset();
    inline AgentRenderNode GetRenderNode();
    glm::vec3 Position;
    
    float Radius = 0.01f;
    
    float Yaw = 0.00, Pitch = 0.0, Roll = 0.00;
    
    static const float MAX_HEALTH;
    float Health = MAX_HEALTH;
    
    bool Active=true;
    
    glm::vec3 Color;
    
    const float MaxSpeed = 0.005f;
    int TimeAlive=0;
    
    glm::vec3 Velocity=glm::vec3(1,0,0);
    
    void Update(std::vector<Plant>& plants, std::vector<Agent>& agents);
    void EatPlant(std::vector<Plant>& plants, size_t index);
    void Mutate();
private:
    Brain brain;
    inline glm::vec3 getOrientation();
};

AgentRenderNode Agent::GetRenderNode() { return AgentRenderNode{Position, Color}; }

glm::vec3 Agent::getOrientation()
{
    return glm::vec3(glm::yawPitchRoll(glm::degrees(Yaw), glm::degrees(Pitch), glm::degrees(Roll)) * glm::vec4(Velocity,1.0f));
}