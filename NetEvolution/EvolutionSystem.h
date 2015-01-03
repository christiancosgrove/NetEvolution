//
//  EvolutionSystem.h
//  NetEvolution
//
//  Created by Christian on 12/21/14.
//  Copyright (c) 2014 Christian Cosgrove. All rights reserved.
//
#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include "Agent.h"
#include <OpenGL/gl3.h>
#include "Plant.h"
#include <string>
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
    std::vector<Agent> agents;
    std::vector<Plant> plants;
    glm::vec3 domainDimensions = glm::vec3(1,1,1);
    void draw();
    void update();
    void handleEvents();
    void initializeAgents();
    void newAgents();
    std::string fileLocation = "/Users/christian/Desktop/performance.csv";
    
    int time=0;
    
    const int plantSpawnDelay=10;
    int lastPlantSpawn=0;
    void pollAgents();
    
    bool accelerate=false;
};