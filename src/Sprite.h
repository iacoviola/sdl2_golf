#ifndef SPRITE_H
#define SPRITE_H

#include <SDL2/SDL.h>

#include "Vector2f.h"
#include "Texture.h"

namespace sdl {

enum sdlDirection {
    SDL_NONE = -1,
    SDL_UP = 0,
    SDL_DOWN = 1,
    SDL_LEFT = 2,
    SDL_RIGHT = 3
};

class Sprite
{
    public:
        Sprite();

        Sprite(sdl::Texture *texture);

        Sprite(sdl::Texture *texture, math::Vector2f position);

        ~Sprite();

        sdlDirection collidesWith(Sprite& other);

        void setTexture(sdl::Texture* texture);

        void setScale(math::Vector2f scale);

        void setScale(float x, float y);

        void setPosition(math::Vector2f position);

        void setPosition(float x, float y);

        void setFlip(SDL_RendererFlip flip);

        void setAngle(float angle);

        void setRotationCenter(SDL_FPoint* center);

        void setRotationCenter(int x, int y);

        void setClip(SDL_Rect* clip);

        void setClip(int x, int y, int w, int h);

        Texture* getTexture();

        math::Vector2f& getScale();

        math::Vector2f getRawScale();

        math::Vector2f& getPosition();

        SDL_RendererFlip& getFlip();

        float getAngle();

        SDL_FPoint* getRotationCenter();

        SDL_Rect* getClip();

        math::Vector2f getCenter();

        protected:
            sdl::Texture* texture;
            math::Vector2f scale;
            math::Vector2f position;
            SDL_RendererFlip flip;
            float angle;
            SDL_FPoint* center;
            SDL_Rect* clip;
};
}

#endif // SPRITE_H