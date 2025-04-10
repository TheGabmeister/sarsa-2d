#ifndef RENDERCOLLIDERSYSTEM_H
#define RENDERCOLLIDERSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"
#include <SDL3/SDL.h>

class RenderColliderSystem: public System {
    public:
        RenderColliderSystem() {
            RequireComponent<TransformComponent>();
            RequireComponent<BoxColliderComponent>();
        }

        void Update(SDL_Renderer* renderer) {
            for (auto entity: GetSystemEntities()) {
                const auto transform = entity.GetComponent<TransformComponent>();
                const auto collider = entity.GetComponent<BoxColliderComponent>();

                SDL_FRect colliderRect = {
                    transform.position.x + collider.offset.x,
                    transform.position.y + collider.offset.y,
                    static_cast<float>(collider.width),
                    static_cast<float>(collider.height)
                };
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderRect(renderer, &colliderRect);
            }
        }
};

#endif
