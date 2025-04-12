#include "Game.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <spdlog/spdlog.h>
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/CameraFollowComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/HealthComponent.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/RenderColliderSystem.h"
#include "../Systems/DamageSystem.h"
#include "../Systems/KeyboardControlSystem.h"
#include "../Systems/CameraMovementSystem.h"
#include "../Systems/ProjectileEmitSystem.h"
#include "../Systems/ProjectileLifecycleSystem.h"
#include "../Events/KeyPressedEvent.h"
#include <glm/glm.hpp>
#include <fstream>

int Game::windowWidth;
int Game::windowHeight;
int Game::mapWidth;
int Game::mapHeight;

Game::Game() 
{
    isRunning = false;
    isDebug = false;
    registry = std::make_unique<Registry>();
    assetStore = std::make_unique<AssetStore>();
    eventBus = std::make_unique<EventBus>();
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
        1280,
        720,
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

    //SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    // game camera initialize
    camera.x = 0;
    camera.y = 0;
    camera.w = windowWidth;
    camera.h = windowHeight;

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
            if (sdlEvent.key.key == SDLK_D) {
                isDebug = !isDebug;
            }
			eventBus->EmitEvent<KeyPressedEvent>(sdlEvent.key.key);
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
    
    eventBus->Reset();
    registry->GetSystem<DamageSystem>().SubscribeToEvents(eventBus);
    registry->GetSystem<KeyboardControlSystem>().SubscribeToEvents(eventBus);
    registry->GetSystem<ProjectileEmitSystem>().SubscribeToEvents(eventBus);

    // Update the registry to process the entities that are waiting to be created/deleted
    registry->Update();

    registry->GetSystem<MovementSystem>().Update(deltaTime);
    registry->GetSystem<AnimationSystem>().Update();
    registry->GetSystem<CollisionSystem>().Update(eventBus);
    //registry->GetSystem<CameraMovementSystem>().Update(camera);
    registry->GetSystem<ProjectileEmitSystem>().Update(registry);
    registry->GetSystem<ProjectileLifecycleSystem>().Update();
}

void Game::Render() 
{
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderClear(renderer);

    // Invoke all the systems that need to render 
    registry->GetSystem<RenderSystem>().Update(renderer, assetStore, camera);
    if (isDebug) {
        registry->GetSystem<RenderColliderSystem>().Update(renderer, camera);
    }
	SDL_RenderPresent(renderer);
}

void Game::LoadLevel(int level)
{
    registry->AddSystem<MovementSystem>();
    registry->AddSystem<RenderSystem>();
    registry->AddSystem<AnimationSystem>();
    registry->AddSystem<CollisionSystem>();
    registry->AddSystem<RenderColliderSystem>();
    registry->AddSystem<DamageSystem>();
    registry->AddSystem<KeyboardControlSystem>();
    registry->AddSystem<CameraMovementSystem>();
    registry->AddSystem<ProjectileEmitSystem>();
    registry->AddSystem<ProjectileLifecycleSystem>();

    // Adding assets to the asset store
    assetStore->AddTexture(renderer, "car-texture", RESOURCES_PATH "textures/car.png");
    assetStore->AddTexture(renderer, "tileset-grass-texture", RESOURCES_PATH "textures/grid.jpg");
    assetStore->AddTexture(renderer, "vampire-texture", RESOURCES_PATH "textures/enemies-vampire-attack.png");
    assetStore->AddTexture(renderer, "ball-texture", RESOURCES_PATH "textures/ball.png");

    // Load the tilemap
    int tileSize = 64;
    double tileScale = 1.0;
    int mapNumCols = 4;
    int mapNumRows = 4;
    
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
            tile.Group("tiles");
            tile.AddComponent<TransformComponent>(
                glm::vec2(x * (tileScale * tileSize), 
                y * (tileScale * tileSize)), 
                glm::vec2(tileScale, tileScale), 
                0.0
            );
            tile.AddComponent<SpriteComponent>("tileset-grass-texture", tileSize, tileSize, 0, false, srcRectX, srcRectY);
        }
    }
    mapFile.close();
    mapWidth = mapNumCols * tileSize * tileScale;
    mapHeight = mapNumRows * tileSize * tileScale;
    
    Entity car = registry->CreateEntity();
    car.Tag("player");
    car.AddComponent<TransformComponent>(glm::vec2(10.0, 30.0), glm::vec2(1.0, 1.0), 0.0);
    car.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    car.AddComponent<SpriteComponent>("car-texture", 256, 128, 1);
    car.AddComponent<BoxColliderComponent>(128,128);
    car.AddComponent<KeyboardControlledComponent>(glm::vec2(0, -100), glm::vec2(100, 0), glm::vec2(0, 100), glm::vec2(-100, 0));
    car.AddComponent<CameraFollowComponent>();
    car.AddComponent<ProjectileEmitterComponent>(glm::vec2(1000.0, 1000.0), 0, 3000, 10, true);
    car.AddComponent<HealthComponent>(100);

    Entity vampire = registry->CreateEntity();
    vampire.Group("enemies");
    vampire.AddComponent<TransformComponent>(glm::vec2(500.0, 100.0), glm::vec2(2.0, 2.0), 0.0);
    vampire.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    vampire.AddComponent<SpriteComponent>("vampire-texture", 32, 32, 1);
    vampire.AddComponent<AnimationComponent>(16, 15, true);
    vampire.AddComponent<BoxColliderComponent>(64, 64);
    vampire.AddComponent<ProjectileEmitterComponent>(glm::vec2(0.0, 100.0), 0, 5000, 10, false);
    vampire.AddComponent<HealthComponent>(100);
}

void Game::Destroy() 
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
