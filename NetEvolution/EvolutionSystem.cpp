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
EvolutionSystem::EvolutionSystem()
{
    if (SDL_Init(SDL_INIT_VIDEO)) throw std::logic_error("Failed to initialize SDL.  " + std::string(SDL_GetError()));
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetSwapInterval(1);
    
    
    window = SDL_CreateWindow("Net Evolution", 0, 0, 1920, 1080, SDL_WINDOW_OPENGL);
    if (window==nullptr) throw std::logic_error("Window failed to be initialized");
    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (context==nullptr) throw std::logic_error("SDL_GL could not be initialized!");
    
    
    GLManager glManager(resourcePath() + "fragmentShader.glsl", resourcePath() + "vertexShader.glsl");
    
    while (CurrentProgramState==ProgramState::RUN)
    {
        update();
        draw();
        handleEvents();
        SDL_GL_SwapWindow(window);
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
    
    
}

void EvolutionSystem::update()
{
    
}