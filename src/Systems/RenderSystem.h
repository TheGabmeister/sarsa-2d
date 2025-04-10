#pragma once

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../AssetStore/AssetStore.h"
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

        void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore)
        {
            // Create a vector with both Sprite and Transform component of all entities
            struct RenderableEntity {
                TransformComponent transformComponent;
                SpriteComponent spriteComponent;
            };
            std::vector<RenderableEntity> renderableEntities;

            for (auto entity : GetSystemEntities()) {
                RenderableEntity renderableEntity;
                renderableEntity.spriteComponent = entity.GetComponent<SpriteComponent>();
                renderableEntity.transformComponent = entity.GetComponent<TransformComponent>();
                renderableEntities.emplace_back(renderableEntity);
            }

            // Sort the vector by the z-index value
            std::sort(renderableEntities.begin(), renderableEntities.end(), [](const RenderableEntity& a, const RenderableEntity& b) {
                return a.spriteComponent.zIndex < b.spriteComponent.zIndex;
            });

            // Loop all entities that the system is interested in
            for (auto entity : renderableEntities) {
                const auto transform = entity.transformComponent;
                const auto sprite = entity.spriteComponent;

                // Set the source rectangle of our original sprite texture
                SDL_FRect srcRect = sprite.srcRect;

                // Set the destination rectangle with the x,y position to be rendered
                SDL_FRect dstRect = {
                    (transform.position.x),
                    (transform.position.y),
                    (sprite.width * transform.scale.x),
                    (sprite.height * transform.scale.y)
                };

                // Render the texture on the destination renderer window
                SDL_RenderTextureRotated(
                    renderer,
                    assetStore->GetTexture(sprite.assetId),
                    &srcRect,
                    &dstRect,
                    transform.rotation,
                    NULL,
                    SDL_FLIP_NONE
                );
            }
        }
};