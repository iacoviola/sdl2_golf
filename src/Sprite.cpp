#include <SDL2/SDL.h>

#include "Sprite.h"

#include "Vector2f.h"
#include "Texture.h"

sdl::Sprite::Sprite()
: texture(nullptr), scale(0, 0), position(0, 0), flip(SDL_FLIP_NONE), angle(0), center(nullptr), clip(nullptr) {}

sdl::Sprite::Sprite(sdl::Texture *texture)
: texture(texture), scale(texture->getWidth(), texture->getHeight()), position(0, 0), flip(SDL_FLIP_NONE), angle(0), center(nullptr), clip(nullptr) {}

sdl::Sprite::Sprite(sdl::Texture *texture, math::Vector2f position)
: texture(texture), scale(texture->getWidth(), texture->getHeight()), position(position), flip(SDL_FLIP_NONE), angle(0), center(nullptr), clip(nullptr) {}

sdl::Sprite::~Sprite(){
    texture = nullptr;
    center = nullptr;
    clip = nullptr;
}

void sdl::Sprite::setTexture(sdl::Texture* texture){
    this->texture = texture;
    setScale(texture->getWidth(), texture->getHeight());
}

void sdl::Sprite::setScale(math::Vector2f scale){
    this->scale = scale;
}

void sdl::Sprite::setScale(float x, float y){
    this->scale.x = x;
    this->scale.y = y;
}

void sdl::Sprite::setPosition(math::Vector2f position){
    this->position = position;
}

void sdl::Sprite::setPosition(float x, float y){
    this->position.x = x;
    this->position.y = y;
}

void sdl::Sprite::setFlip(SDL_RendererFlip flip){
    this->flip = flip;
}

void sdl::Sprite::setAngle(float angle){
    this->angle = angle;
}

void sdl::Sprite::setRotationCenter(SDL_FPoint* center){
    if(this->center == nullptr)
        this->center = new SDL_FPoint();
    this->center = center;
}

void sdl::Sprite::setRotationCenter(int x, int y){
    if(this->center == nullptr)
        this->center = new SDL_FPoint();
    this->center->x = x;
    this->center->y = y;
}

void sdl::Sprite::setClip(SDL_Rect* clip){
    if(this->clip == nullptr)
        this->clip = new SDL_Rect();
    this->clip = clip;
}

void sdl::Sprite::setClip(int x, int y, int w, int h){
    if(this->clip == nullptr)
        this->clip = new SDL_Rect();
    this->clip->x = x;
    this->clip->y = y;
    this->clip->w = w;
    this->clip->h = h;
}

sdl::Texture* sdl::Sprite::getTexture(){
    return texture;
}

math::Vector2f& sdl::Sprite::getScale(){
    return scale;
}

math::Vector2f sdl::Sprite::getRawScale(){
    return math::Vector2f(texture->getWidth(), texture->getHeight());
}

math::Vector2f& sdl::Sprite::getPosition(){
    return position;
}

SDL_RendererFlip& sdl::Sprite::getFlip(){
    return flip;
}

float sdl::Sprite::getAngle(){
    return angle;
}

SDL_FPoint* sdl::Sprite::getRotationCenter(){
    return center;
}

SDL_Rect* sdl::Sprite::getClip(){
    return clip;
}

math::Vector2f sdl::Sprite::getCenter() {
    return math::Vector2f(position.x + (scale.x / 2), position.y + (scale.y / 2));
}