//
//  EvolutionSystem.h
//  NetEvolution
//
//  Created by Christian on 12/21/14.
//  Copyright (c) 2014 Christian Cosgrove. All rights reserved.
//
#include <SDL2/SDL.h>
#include <vector>
#include "Agent.h"
#include <OpenGL/gl3.h>
#pragma once
class EvolutionSystem
{
public:
    
    enum class ProgramState
    {
        RUN, EXIT
    } CurrentProgramState = ProgramState::RUN;
    
    EvolutionSystem();
private:
    SDL_Window* window;
    GLuint vbo, vao;
    
    void generateBuffers();
    void updateBuffers();
    std::vector<AgentRenderNode> renderNodes;
    void draw();
    void update();
    void handleEvents();
};