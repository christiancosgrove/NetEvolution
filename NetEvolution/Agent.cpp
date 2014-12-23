//
//  Agent.cpp
//  NetEvolution
//
//  Created by Christian on 12/21/14.
//  Copyright (c) 2014 Christian Cosgrove. All rights reserved.
//

#include "Agent.h"
#include "glm/gtx/euler_angles.hpp"

Agent::Agent(glm::vec3 position) : Position(position)
{
    
}

void Agent::Update()
{
    Velocity = glm::vec3(glm::yawPitchRoll(Yaw, Pitch, Roll) * glm::vec4(Velocity,1));
    Position+=MaxSpeed*Velocity;
}

