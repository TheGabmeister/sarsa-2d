#pragma once

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include <spdlog/spdlog.h>
#include <SDL3/SDL.h>  

class RenderSystem: public System 
{
    public:
        RenderSystem() 
        {
            RequireComponent<TransformComponent>();
            RequireComponent<SpriteComponent>();
        }

        void Update(SDL_Renderer* renderer) 
        {
            // Loop all entities that the system is interested in
            for (auto entity: GetSystemEntities()) 
            {
				const auto transform = entity.GetComponent<TransformComponent>();
                const auto sprite = entity.GetComponent<SpriteComponent>();
				
				SDL_FRect objRect = {
					static_cast<int>(transform.position.x),
					static_cast<int>(transform.position.y),
					sprite.width,
					sprite.height
				};

				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				SDL_RenderFillRect(renderer, &objRect);
                
            }
        }
};