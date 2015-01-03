//
//  EvolutionSystem.cpp
//  NetEvolution
//
//  Created by Christian on 12/21/14.
//  Copyright (c) 2014 Christian Cosgrove. All rights reserved.
//

#include "EvolutionSystem.h"
#include <stdexcept>
#include <string>
#include "GLManager.h"
#include "ResourcePath.hpp"
#include "Camera.h"
#include "RandomUtils.h"
#include <fstream>break
EvolutionSystem::EvolutionSystem()
{
    if (SDL_Init(SDL_INIT_VIDEO)) throw std::logic_error("Failed to initialize SDL.  " + std::string(SDL_GetError()));
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetSwapInterval(1);
    
    
    window = SDL_CreateWindow("Net Evolution", 0, 0, 1280,720, SDL_WINDOW_OPENGL);
    if (window==nullptr) throw std::logic_error("Window failed to be initialized");
    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (context==nullptr) throw std::logic_error("SDL_GL could not be initialized!");
    
    
    SDL_SetRelativeMouseMode(SDL_TRUE);
    Camera camera(1280,720);
    
    GLManager glManager(resourcePath() + "fragmentShader.glsl", resourcePath() + "vertexShader.glsl");
    initializeAgents();
    generateBuffers();
    
    {
        std::ofstream stream(fileLocation, std::ios::out);
        stream << "time,avg talive,max talive,avg health,max health" << std::endl;
    
    }
    while (CurrentProgramState==ProgramState::RUN)
    {
        glManager.Programs[0].SetMatrix4("transformMatrix", glm::value_ptr(camera.GetTransformMatrix()));
        
        if (accelerate)
            for (int i = 0; i<3000;i++)
            update();
        else update();
        camera.Update();
        draw();
        handleEvents();
        SDL_GL_SwapWindow(window);
    }
    
}

void EvolutionSystem::initializeAgents()
{
    const int agentCount = 100;
    for (int i = 0; i<agentCount;i++)
    {
        agents.push_back(Agent(
            glm::vec3(
                      RandomUtils::Uniform<float>(0, domainDimensions.x),
                      RandomUtils::Uniform<float>(0, domainDimensions.y),
                      RandomUtils::Uniform<float>(0, domainDimensions.z))));
    }
    
    const int plantCount = 100;
    for (int i = 0; i<plantCount;i++)
    {
        plants.push_back(Plant(
            glm::vec3(
                      RandomUtils::Uniform<float>(0, domainDimensions.x),
                      RandomUtils::Uniform<float>(0, domainDimensions.y),
                      RandomUtils::Uniform<float>(0, domainDimensions.z))));
    }
}


void EvolutionSystem::handleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                CurrentProgramState = ProgramState::EXIT;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode)
            {
                case SDL_SCANCODE_ESCAPE:
                    CurrentProgramState = ProgramState::EXIT;
                    break;
                case SDL_SCANCODE_SPACE:
                    accelerate=!accelerate;
                    break;
                default: break;
            }
                
            default: break;
        }
    }
}

void EvolutionSystem::draw()
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    updateBuffers();
    
    glBindVertexArray(vao);
    glDrawArrays(GL_POINTS, 0, renderNodes.size());
    glBindVertexArray(0);
}

void EvolutionSystem::update()
{
    for (Agent& agent:agents)
    {
        agent.Update(plants, agents);
    }
    for (Plant& plant:plants)
    {
        plant.Update();
    }
    for (Agent& agent:agents)
    {
        if (agent.Position.x<0) agent.Position.x = domainDimensions.x + agent.Position.x;
        if (agent.Position.y<0) agent.Position.y = domainDimensions.y + agent.Position.y;
        if (agent.Position.z<0) agent.Position.z = domainDimensions.z + agent.Position.z;
        
        if (agent.Position.x>domainDimensions.x) agent.Position.x = agent.Position.x - domainDimensions.x;
        if (agent.Position.y>domainDimensions.y) agent.Position.y = agent.Position.y - domainDimensions.y;
        if (agent.Position.z>domainDimensions.z) agent.Position.z = agent.Position.z - domainDimensions.z;
    }
    time++;
    lastPlantSpawn--;
    if (time%(int)Agent::MAX_HEALTH==0) pollAgents();
    
    newAgents();
}

void EvolutionSystem::generateBuffers()
{
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(AgentRenderNode), (void*)__offsetof(AgentRenderNode, position));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(AgentRenderNode), (void*)__offsetof(AgentRenderNode, color));
    glBindVertexArray(0);
}

void EvolutionSystem::updateBuffers()
{
    renderNodes.clear();
    renderNodes.reserve(agents.size());
    for (Agent& agent:agents) if (agent.Active) renderNodes.push_back(agent.GetRenderNode());
    for (Plant& plant:plants) if (plant.Active) renderNodes.push_back(plant.GetRenderNode());
    
    glBindVertexArray(vao);
    glBufferData(GL_ARRAY_BUFFER, sizeof(AgentRenderNode) * renderNodes.size(), &renderNodes[0], GL_DYNAMIC_DRAW);
    glBindVertexArray(0);
}
#include <iostream>
void EvolutionSystem::newAgents()
{
    std::vector<float> probabilities(agents.size());
    std::vector<float> intervals(agents.size());
    for (int i = 0; i<agents.size();i++) intervals[i]=i;
    
    int aliveAgents = 0;
    
    float totalTimeAlive = 0;
    for (int i = 0; i<agents.size();i++)
    {
        probabilities[i] = agents[i].TimeAlive;
        totalTimeAlive+=agents[i].TimeAlive;
        if (agents[i].Active) aliveAgents++;
    }
    
    for(float& i : probabilities) i/=totalTimeAlive;
    
    std::piecewise_constant_distribution<float> distrib(intervals.begin(), intervals.end(), probabilities.begin());
//    for (float i:probabilities) std::cout << i << " "; std::cout << std::endl;
    
    
    for (int i = 0; i<agents.size();i++)
    {
        Agent& agent = agents[i];
        if (!agent.Active)
        {
            if (aliveAgents>0)
            {
                float randInd =distrib(RandomUtils::rand);
//                std::cout<<randInd<<std::endl;
                agent.Duplicate(agents[(int)randInd]);
                
                agent.Position =
                glm::vec3(
                          RandomUtils::Uniform<float>(0, domainDimensions.x),
                          RandomUtils::Uniform<float>(0, domainDimensions.y),
                          RandomUtils::Uniform<float>(0, domainDimensions.z));
            }
            else
            {
                agent.Reset();
                agent.Position =
                glm::vec3(
                          RandomUtils::Uniform<float>(0, domainDimensions.x),
                          RandomUtils::Uniform<float>(0, domainDimensions.y),
                          RandomUtils::Uniform<float>(0, domainDimensions.z));
            }
        }
    }
    for (Plant& plant:plants)
    {
        if (!plant.Active && lastPlantSpawn<=0)
        {
            plant.Position =
            glm::vec3(
                      RandomUtils::Uniform<float>(0, domainDimensions.x),
                      RandomUtils::Uniform<float>(0, domainDimensions.y),
                      RandomUtils::Uniform<float>(0, domainDimensions.z));
            plant.Health=plant.MAX_HEALTH;
            plant.Active=true;
            lastPlantSpawn=plantSpawnDelay;
        }
    }
    
}

void EvolutionSystem::pollAgents()
{
    float totTimeAlive=0, maxTimeAlive=00, totHealth=0, maxHealth;
    for(Agent& agent:agents)
    {
        totHealth+=agent.Health;
        totTimeAlive+=agent.TimeAlive;
        if (agent.Health>maxHealth) maxHealth=agent.Health;
        if (agent.TimeAlive>maxTimeAlive) maxTimeAlive=agent.TimeAlive ;
    }
    std::ofstream stream(fileLocation, std::ios::out | std::ios::app);
    stream  << time << "," << totTimeAlive/agents.size() << "," << maxTimeAlive << "," << totHealth / agents.size() << maxHealth << std::endl;
    printf("avg talive: %f, max talive: %f, avg health: %f, max health %f\n", totTimeAlive/agents.size(), maxTimeAlive, totHealth / agents.size(), maxHealth);
}