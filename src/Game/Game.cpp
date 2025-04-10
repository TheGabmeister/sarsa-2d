#include "Game.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <spdlog/spdlog.h>
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/CollisionSystem.h"
#include <glm/glm.hpp>
#include <fstream>

Game::Game() 
{
    isRunning = false;
    registry = std::make_unique<Registry>();
    assetStore = std::make_unique<AssetStore>();
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
    LoadLevel(1);
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
    
    // Update the registry to process the entities that are waiting to be created/deleted
    registry->Update();

    registry->GetSystem<MovementSystem>().Update(deltaTime);
    registry->GetSystem<AnimationSystem>().Update();
    registry->GetSystem<CollisionSystem>().Update();
}

void Game::Render() 
{
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderClear(renderer);

    // Invoke all the systems that need to render 
    registry->GetSystem<RenderSystem>().Update(renderer, assetStore);

	SDL_RenderPresent(renderer);
}

void Game::LoadLevel(int level)
{
    registry->AddSystem<MovementSystem>();
    registry->AddSystem<RenderSystem>();
    registry->AddSystem<AnimationSystem>();
    registry->AddSystem<CollisionSystem>();

    // Adding assets to the asset store
    assetStore->AddTexture(renderer, "car-texture", RESOURCES_PATH "textures/car.png");
    assetStore->AddTexture(renderer, "tileset-grass-texture", RESOURCES_PATH "textures/grid.jpg");
    assetStore->AddTexture(renderer, "vampire-texture", RESOURCES_PATH "textures/enemies-vampire-attack.png");

    // Load the tilemap
    int tileSize = 64;
    double tileScale = 1.0;
    int mapNumCols = 4;
    int mapNumRows = 4;
    /*
    std::fstream mapFile;
    mapFile.open(RESOURCES_PATH "level-01.map");

    for (int y = 0; y < mapNumRows; y++) {
        for (int x = 0; x < mapNumCols; x++) {
            char ch;
            mapFile.get(ch);
            int srcRectY = std::atoi(&ch) * tileSize;
            mapFile.get(ch);
            int srcRectX = std::atoi(&ch) * tileSize;
            mapFile.ignore();

            Entity tile = registry->CreateEntity();
            tile.AddComponent<TransformComponent>(glm::vec2(x * (tileScale * tileSize), y * (tileScale * tileSize)), glm::vec2(tileScale, tileScale), 0.0);
            tile.AddComponent<SpriteComponent>("tileset-grass-texture", tileSize, tileSize, srcRectX, srcRectY);
        }
    }
    mapFile.close();
    */
    Entity car = registry->CreateEntity();
    car.AddComponent<TransformComponent>(glm::vec2(10.0, 30.0), glm::vec2(1.0, 1.0), 0.0);
    car.AddComponent<RigidBodyComponent>(glm::vec2(50.0, 0.0));
    car.AddComponent<SpriteComponent>("car-texture", 256, 128);
    car.AddComponent<BoxColliderComponent>(128,128);

    Entity vampire = registry->CreateEntity();
    vampire.AddComponent<TransformComponent>(glm::vec2(500.0, 0.0), glm::vec2(5.0, 5.0), 0.0);
    vampire.AddComponent<RigidBodyComponent>(glm::vec2(-30.0, 0.0));
    vampire.AddComponent<SpriteComponent>("vampire-texture", 32, 32, 1);
    vampire.AddComponent<AnimationComponent>(16, 15, true);
    vampire.AddComponent<BoxColliderComponent>(128, 128);
}

void Game::Destroy() 
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
