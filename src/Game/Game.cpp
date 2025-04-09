#include "Game.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <spdlog/spdlog.h>
#include <./Components/TransformComponent.h>
#include <./Components/RigidBodyComponent.h>
#include <glm/glm.hpp>

Game::Game() 
{
    isRunning = false;
    registry = std::make_unique<Registry>();
    spdlog::info("Game constructor called!");
}

Game::~Game() 
{
    spdlog::info("Game destructor called!");
}


void Game::Initialize() 
{
    if (SDL_Init(SDL_INIT_VIDEO) != true) 
    {
        spdlog::critical("Error initializing SDL.");
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
        spdlog::critical("Error creating SDL window.");
        return;
    }

    renderer = SDL_CreateRenderer(window, NULL);

    if (!renderer) 
    {
        spdlog::critical("Error creating SDL renderer.");
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
	Entity car = registry->CreateEntity();
	car.AddComponent<TransformComponent>(glm::vec2(10.0, 30.0), glm::vec2(1.0,1.0), 0.0);
    car.AddComponent<RigidBodyComponent>(glm::vec2(10.0, 50.0));

	car.RemoveComponent<TransformComponent>();
}

void Game::Update() 
{
    // If we are too fast, waste some time until we reach the MILLISECS_PER_FRAME
    int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecsPreviousFrame);
    if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME) {
        SDL_Delay(timeToWait);
    }

    // The difference in ticks since the last frame, converted to seconds
    double deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0;

    // Store the "previous" frame time
    millisecsPreviousFrame = SDL_GetTicks();

    // TODO:
    // MovementSystem.Update();
    // CollisionSystem.Update();
    // DamageSystem.Update();
}

void Game::Render() 
{
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderClear(renderer);

    std::string imagePath = RESOURCES_PATH "textures/car.png";

    // Loads a PNG texture
    SDL_Surface* surface = IMG_Load(imagePath.c_str());
    if (surface == nullptr) 
    {
        spdlog::warn("IMG_Load Error: {}", SDL_GetError());
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    if (texture == nullptr)
    {
        spdlog::warn("SDL_CreateTextureFromSurface Error: {}", SDL_GetError());
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
