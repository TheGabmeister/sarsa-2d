#include "./AssetStore.h"
#include "spdlog/spdlog.h"
#include <SDL3_image/SDL_image.h>

AssetStore::AssetStore() {
    spdlog::info("AssetStore constructor called!");
}

AssetStore::~AssetStore() {
    ClearAssets();
    spdlog::info("AssetStore destructor called!");
}

void AssetStore::ClearAssets() {
    for (auto texture: textures) {
        SDL_DestroyTexture(texture.second);
    }
    textures.clear();
}

void AssetStore::AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath) {
    SDL_Surface* surface = IMG_Load(filePath.c_str());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);

    // Add the texture to the map
    textures.emplace(assetId, texture);

    spdlog::info("New texture added to the Asset Store with id = " + assetId);
}

SDL_Texture* AssetStore::GetTexture(const std::string& assetId) {
    return textures[assetId];
}
