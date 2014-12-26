//
//  Plant.cpp
//  NetEvolution
//
//  Created by Christian on 12/22/14.
//  Copyright (c) 2014 Christian Cosgrove. All rights reserved.
//

#include "Plant.h"

const float Plant::MAX_HEALTH = 100.0f;


Plant::Plant(glm::vec3 position) : Position(position) {}

void Plant::Update()
{
    Health-=0.01f;
    if (Health<=0) Active=false;
}