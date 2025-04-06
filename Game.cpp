#include "Game.h"
#include <SDL3/SDL.h>
#include <iostream>

Game::Game() 
{
    isRunning = false;
}

Game::~Game() 
{
}

void Game::Initialize() 
{
    if (SDL_Init(SDL_INIT_VIDEO) != true) 
    {
        std::cerr << "Error initializing SDL." << std::endl;
        return;
    }

    window = SDL_CreateWindow(
        "GameWindow",
        800,
        600,
        SDL_WINDOW_RESIZABLE
    );
    if (!window) 
    {
        std::cerr << "Error creating SDL window." << std::endl;
        return;
    }

    renderer = SDL_CreateRenderer(window, NULL);

    if (!renderer) 
    {
        std::cerr << "Error creating SDL renderer." << std::endl;
        return;
    }
    //SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    isRunning = true;
}

void Game::ProcessInput() 
{
    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent)) {
        switch (sdlEvent.type) {
        case SDL_EVENT_QUIT:
            isRunning = false;
            break;
        case SDL_EVENT_KEY_DOWN:
            if (sdlEvent.key.down == SDLK_ESCAPE) {
                isRunning = false;
            }
            break;
        }
    }
}

void Game::Setup() 
{
    
}

void Game::Update() 
{
    
}

void Game::Render() 
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_RenderPresent(renderer);
}

void Game::Run() 
{
    Setup();
    while (isRunning) 
    {
        ProcessInput();
        Update();
        Render();
    }
}

void Game::Destroy() 
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
