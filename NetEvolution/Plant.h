//
//  Plant.h
//  NetEvolution
//
//  Created by Christian on 12/22/14.
//  Copyright (c) 2014 Christian Cosgrove. All rights reserved.
//
#pragma once
#include "glm/glm.hpp"
#include "AgentRenderNode.h"
class Plant
{
public:
    Plant(glm::vec3 position);
    
    glm::vec3 Position;
    static const float MAX_HEALTH;
    float Health = MAX_HEALTH;
    float Radius =0.01f;
    bool Active=true;
    inline AgentRenderNode GetRenderNode();
    void Update();
private:
};

AgentRenderNode Plant::GetRenderNode() { return AgentRenderNode{Position, glm::vec3(0,1,0)}; }