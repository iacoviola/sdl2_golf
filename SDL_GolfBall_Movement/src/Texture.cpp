#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

#include "Texture.h"

#include "Vector2f.h"

sdl::Texture::Texture(SDL_Renderer* renderer) 
: texture(nullptr), renderer(renderer) {};

sdl::Texture::~Texture(){
    free();
}

int sdl::Texture::loadFromFile(const std::string path) {
    free();
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if(loadedSurface == nullptr){
        return 0;
    }
    
    //SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
    
    texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if(texture == nullptr){
        return 0;
    }
    size.x = loadedSurface->w;
    size.y = loadedSurface->h;
    SDL_FreeSurface(loadedSurface);
    
    return 1;
}

void sdl::Texture::free(){
    if(texture != nullptr){
        SDL_DestroyTexture(texture);
        texture = nullptr;
        size = math::Vector2f();
    }
}

SDL_Texture* sdl::Texture::getTexture() const {
    return texture;
}

float sdl::Texture::getWidth() const {
    return size.x;
}

float sdl::Texture::getHeight() const {
    return size.y;
}

math::Vector2f sdl::Texture::getSize() const {
    return size;
}