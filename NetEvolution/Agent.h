
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

struct AgentRenderNode
{
    glm::vec3 position;
    
};

class Agent
{
public:
    Agent(glm::vec3 position);
    inline AgentRenderNode GetRenderNode();
    glm::vec3 Position;
    
    float Yaw = 0.01, Pitch = 0.0, Roll = 0.01;
    
    const float MaxSpeed = 0.001f;
    
    glm::vec3 Velocity=glm::vec3(1,0,0);
    
    void Update();
private:
    
};

AgentRenderNode Agent::GetRenderNode() { return AgentRenderNode{Position}; }