#pragma once

#include <./ECS/ECS.h>
#include <SDL3/SDL.h>

class Game 
{
    private:
        bool isRunning;
        SDL_Window* window;
        SDL_Renderer* renderer;

        std::unique_ptr<Registry> registry;

    public:
        Game();
        ~Game();
        void Initialize();
        void Run();
        void Setup();
        void ProcessInput();
        void Update();
        void Render();
        void Destroy();
};