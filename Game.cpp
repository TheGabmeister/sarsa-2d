#include "Game.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

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

void Game::ProcessInput() 
{
    SDL_Event sdlEvent;

    while (SDL_PollEvent(&sdlEvent)) {
        switch (sdlEvent.type) {
        case SDL_EVENT_QUIT:
            isRunning = false;
            break;
        case SDL_EVENT_KEY_DOWN:
            if (sdlEvent.key.key == SDLK_ESCAPE) {
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
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderClear(renderer);

    fs::path texturePath = fs::path("..") / "assets" / "textures" / "car.png";
    std::string pathStr = texturePath.string(); // Converts to OS-native format

    // Loads a PNG texture
    SDL_Surface* surface = IMG_Load(pathStr.c_str());
    if (surface == nullptr) 
    {
        std::cerr << "IMG_Load Error: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    if (texture == nullptr)
    {
        std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
        return;
    }

    // What is the destination rectangle that we want to place our texture
    SDL_FRect dstRect = { 10, 10, 32, 32 };
    SDL_RenderTexture(renderer, texture, NULL, &dstRect);
    SDL_DestroyTexture(texture);

    SDL_RenderPresent(renderer);
}



void Game::Destroy() 
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
