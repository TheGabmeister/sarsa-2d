#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <string>
#include <SDL3/SDL.h>

struct SpriteComponent {
    std::string assetId;
    int width;
    int height;
    int zIndex;
    SDL_FlipMode flip;
    bool isFixed;
    SDL_FRect srcRect;
    
    SpriteComponent(std::string assetId = "", float width = 0, float height = 0, int zIndex = 0, bool isFixed = false, float srcRectX = 0, float srcRectY = 0) {
        this->assetId = assetId;
        this->width = width;
        this->height = height;
        this->zIndex = zIndex;
        this->flip = SDL_FLIP_NONE;
        this->isFixed = isFixed;
        this->srcRect = {srcRectX, srcRectY, width, height};
    }
};

#endif
