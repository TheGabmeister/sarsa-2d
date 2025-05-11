#include "./Game.h"
#include "./LevelLoader.h"
#include "../Logger/Logger.h"
#include "../ECS/ECS.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/CameraMovementSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/RenderColliderSystem.h"
#include "../Systems/DamageSystem.h"
#include "../Systems/ProjectileEmitSystem.h"
#include "../Systems/KeyboardControlSystem.h"
#include "../Systems/ProjectileLifecycleSystem.h"
#include "../Systems/RenderTextSystem.h"
#include "../Systems/RenderHealthBarSystem.h"
#include "../Systems/RenderGUISystem.h"
#include "../Systems/ScriptSystem.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <glm/glm.hpp>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>

int Game::windowWidth;
int Game::windowHeight;
int Game::mapWidth;
int Game::mapHeight;

Game::Game() {
    isRunning = false;
    isDebug = false;
    registry = std::make_unique<Registry>();
    assetStore = std::make_unique<AssetStore>();
    eventBus = std::make_unique<EventBus>();
    Logger::Log("Game constructor called!");
}

Game::~Game() {
    Logger::Log("Game destructor called!");   
}

void Game::Initialize() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == 0) {
        Logger::Err("Error initializing SDL.");

        return;
    }
    if (TTF_Init() == 0) {
        Logger::Err("Error initializing SDL TTF.");
        return;
    }

    //SDL_DisplayMode displayMode;
    //SDL_GetCurrentDisplayMode(0, &displayMode);
    //windowWidth = displayMode.w;
    //windowHeight = displayMode.h;
    windowWidth = 1920;
    windowHeight = 1080;
    window = SDL_CreateWindow(
        "pikuma-2d-engine",
        windowWidth,
        windowHeight,
        SDL_WINDOW_BORDERLESS
    );
    if (!window) {
        Logger::Err("Error creating SDL window.");
        return;
    }
    renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        Logger::Err("Error creating SDL renderer.");
        return;
    }

    // Initialize the ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls


    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);

    // Initialize the camera view with the entire screen area
    camera = {0, 0, windowWidth, windowHeight};

    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    
    isRunning = true;
}

void Game::ProcessInput() {
    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent)) {
        // ImGui SDL input
        ImGui_ImplSDL3_ProcessEvent(&sdlEvent);
        ImGuiIO& io = ImGui::GetIO();
        float mouseX, mouseY;
        const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
        io.MousePos = ImVec2(mouseX, mouseY);
        io.MouseDown[0] = buttons & SDL_BUTTON_MASK(SDL_BUTTON_LEFT);
        io.MouseDown[1] = buttons & SDL_BUTTON_MASK(SDL_BUTTON_RIGHT);

        // Handle core SDL events (close window, key pressed, etc.)
        switch (sdlEvent.type) {
            case SDL_EVENT_QUIT:
                isRunning = false;
                break;
            case SDL_EVENT_KEY_DOWN:
                if (sdlEvent.key.key == SDLK_ESCAPE) {
                    isRunning = false;
                }
                if (sdlEvent.key.key == SDLK_F1) {
                    isDebug = !isDebug;
                }
                eventBus->EmitEvent<KeyPressedEvent>(sdlEvent.key.key);
                break;
        }
    }
}

void Game::Setup() {
    // Add the sytems that need to be processed in our game
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
    registry->AddSystem<RenderTextSystem>();
    registry->AddSystem<RenderHealthBarSystem>();
    registry->AddSystem<RenderGUISystem>();
    registry->AddSystem<ScriptSystem>();

    // Create the bindings between C++ and Lua
    registry->GetSystem<ScriptSystem>().CreateLuaBindings(lua);

    // Load the first level
    LevelLoader loader;
    lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::os);
    loader.LoadLevel(lua, registry, assetStore, renderer, 2);
}

void Game::Update() {
    // The difference in ticks since the last frame, converted to seconds
    double deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0;

    // Store the "previous" frame time
    millisecsPreviousFrame = SDL_GetTicks();
   
    // Reset all event handlers for the current frame
    eventBus->Reset();

    // Perform the subscription of the events for all systems
    registry->GetSystem<MovementSystem>().SubscribeToEvents(eventBus);
    registry->GetSystem<DamageSystem>().SubscribeToEvents(eventBus);
    registry->GetSystem<KeyboardControlSystem>().SubscribeToEvents(eventBus);
    registry->GetSystem<ProjectileEmitSystem>().SubscribeToEvents(eventBus);

    // Update the registry to process the entities that are waiting to be created/deleted
    registry->Update();
    
    // Invoke all the systems that need to update 
    registry->GetSystem<MovementSystem>().Update(deltaTime);
    registry->GetSystem<AnimationSystem>().Update();
    registry->GetSystem<CollisionSystem>().Update(eventBus);
    registry->GetSystem<ProjectileEmitSystem>().Update(registry);
    registry->GetSystem<CameraMovementSystem>().Update(camera);
    registry->GetSystem<ProjectileLifecycleSystem>().Update();
    registry->GetSystem<ScriptSystem>().Update(deltaTime, SDL_GetTicks());
}

void Game::Render() {
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    SDL_RenderClear(renderer);

    // Invoke all the systems that need to render 
    registry->GetSystem<RenderSystem>().Update(renderer, assetStore, camera);
    registry->GetSystem<RenderTextSystem>().Update(renderer, assetStore, camera);
    registry->GetSystem<RenderHealthBarSystem>().Update(renderer, assetStore, camera);
    //if (isDebug) {
        //registry->GetSystem<RenderColliderSystem>().Update(renderer, camera);
        registry->GetSystem<RenderGUISystem>().Update(registry, camera, renderer);
    //}

    SDL_RenderPresent(renderer);
}

void Game::Run() {
    Setup();
    while (isRunning) {
        ProcessInput();
        Update();
        Render();
    }
}

void Game::Destroy() {
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
