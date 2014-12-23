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
EvolutionSystem::EvolutionSystem()
{
    if (SDL_Init(SDL_INIT_VIDEO)) throw std::logic_error("Failed to initialize SDL.  " + std::string(SDL_GetError()));
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetSwapInterval(1);
    
    
    SDL_SetRelativeMouseMode(SDL_TRUE);
    
    window = SDL_CreateWindow("Net Evolution", 0, 0, 1280,720, SDL_WINDOW_OPENGL);
    if (window==nullptr) throw std::logic_error("Window failed to be initialized");
    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (context==nullptr) throw std::logic_error("SDL_GL could not be initialized!");
    
    Camera camera(1280,720);
    
    GLManager glManager(resourcePath() + "fragmentShader.glsl", resourcePath() + "vertexShader.glsl");
    initializeAgents();
    generateBuffers();
    while (CurrentProgramState==ProgramState::RUN)
    {
        glManager.Programs[0].SetMatrix4("transformMatrix", glm::value_ptr(camera.GetTransformMatrix()));
        update();
        camera.Update();
        draw();
        handleEvents();
        SDL_GL_SwapWindow(window);
    }
    
}

void EvolutionSystem::initializeAgents()
{
    const int w = 10;
    const float m = 0.1f;
    for (int i = 0; i<w;i++)
    {
        for (int j = 0; j<w;j++)
        {
            for (int k = 0; k<w;k++)
            {
                agents.push_back(Agent(glm::vec3(m*i,m*j,m*k)));
            }
        }
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
        agent.Update();
    }
    for (Agent& agent:agents)
    {
        if (agent.Position.x<0 || agent.Position.x>domainDimensions.x) agent.Position.x = domainDimensions.x - agent.Position.x;
        if (agent.Position.y<0 || agent.Position.x>domainDimensions.y) agent.Position.y = domainDimensions.y - agent.Position.y;
        if (agent.Position.z<0 || agent.Position.x>domainDimensions.z) agent.Position.z = domainDimensions.z - agent.Position.z;
    }
}

void EvolutionSystem::generateBuffers()
{
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(AgentRenderNode), (void*)__offsetof(AgentRenderNode, position));
    glBindVertexArray(0);
}

void EvolutionSystem::updateBuffers()
{
    renderNodes.clear();
    renderNodes.reserve(agents.size());
    for (Agent& agent:agents) renderNodes.push_back(agent.GetRenderNode());
    
    glBindVertexArray(vao);
    glBufferData(GL_ARRAY_BUFFER, sizeof(AgentRenderNode) * renderNodes.size(), &renderNodes[0], GL_DYNAMIC_DRAW);
    glBindVertexArray(0);
}