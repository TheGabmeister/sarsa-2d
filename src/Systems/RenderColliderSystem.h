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

        void Update(SDL_Renderer* renderer, const SDL_Rect& camera) {
            for (auto entity: GetSystemEntities()) {
                const auto transform = entity.GetComponent<TransformComponent>();
                const auto collider = entity.GetComponent<BoxColliderComponent>();

                // Bypass rendering if entities are outside the camera view
                bool isOutsideCameraView = (
                    transform.position.x + (transform.scale.x * collider.width) < camera.x ||
                    transform.position.x > camera.x + camera.w ||
                    transform.position.y + (transform.scale.y * collider.height) < camera.y ||
                    transform.position.y > camera.y + camera.h
                );

                if (isOutsideCameraView) {
                    continue;
                }

                SDL_FRect colliderRect = {
                    transform.position.x + collider.offset.x - camera.x,
                    transform.position.y + collider.offset.y - camera.y,
                    collider.width * transform.scale.x,
                    collider.height * transform.scale.y
                };
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderRect(renderer, &colliderRect);
            }
        }
};

#endif
