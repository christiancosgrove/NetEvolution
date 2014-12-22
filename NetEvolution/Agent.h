
//
//  Agent.h
//  NetEvolution
//
//  Created by Christian on 12/21/14.
//  Copyright (c) 2014 Christian Cosgrove. All rights reserved.
//
#include "glm/glm.hpp"

#pragma once

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
    
    void Update();
private:
    
};

AgentRenderNode Agent::GetRenderNode() { return AgentRenderNode{Position}; }