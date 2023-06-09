#include <SDL2/SDL.h>

#include "RenderWindow.h"

#include "Sprite.h"
#include "Vector2f.h"
#include "Texture.h"

sdl::RenderWindow::RenderWindow(const std::string title, const int width, const int height) : title(title), size(width, height) {
    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

sdl::RenderWindow::~RenderWindow(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void sdl::RenderWindow::clear(){
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);
}

void sdl::RenderWindow::render(sdl::Sprite& sprite){
    SDL_Rect rect;
    rect.x = sprite.getPosition().x;
    rect.y = sprite.getPosition().y;
    rect.w = sprite.getScale().x;
    rect.h = sprite.getScale().y;
    SDL_RenderCopyEx(renderer, sprite.getTexture()->getTexture(), sprite.getClip(), &rect, sprite.getAngle(), sprite.getCenter(), sprite.getFlip());
}

sdl::Texture* sdl::RenderWindow::loadTextureFromFile(const std::string path){
    sdl::Texture* texture = new sdl::Texture(renderer);
    if(!texture->loadFromFile(path)){
        throw std::runtime_error("Failed to load texture from file");
    }
    return texture;
}

void sdl::RenderWindow::display(){
    SDL_RenderPresent(renderer);
}

SDL_Renderer* sdl::RenderWindow::getRenderer() const {
    return renderer;
}

int sdl::RenderWindow::getWidth() const {
    return size.x;
}

int sdl::RenderWindow::getHeight() const {
    return size.y;
}