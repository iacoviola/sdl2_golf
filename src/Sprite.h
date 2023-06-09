#ifndef SPRITE_H
#define SPRITE_H

#include <SDL2/SDL.h>

#include "Vector2f.h"
#include "Texture.h"

namespace sdl {
class Sprite
{
    public:
        Sprite();

        Sprite(sdl::Texture *texture);

        Sprite(sdl::Texture *texture, math::Vector2f position);

        ~Sprite();

        void setScale(math::Vector2f scale);

        void setScale(float x, float y);

        void setPosition(math::Vector2f position);

        void setPosition(float x, float y);

        void setFlip(SDL_RendererFlip flip);

        void setAngle(float angle);

        void setCenter(SDL_Point* center);

        void setCenter(int x, int y);

        void setClip(SDL_Rect* clip);

        void setClip(int x, int y, int w, int h);

        Texture* getTexture();

        math::Vector2f getScale();

        math::Vector2f getPosition();

        SDL_RendererFlip getFlip();

        float getAngle();

        SDL_Point* getCenter();

        SDL_Rect* getClip();

        private:
            sdl::Texture* texture;
            math::Vector2f scale;
            math::Vector2f position;
            SDL_RendererFlip flip;
            float angle;
            SDL_Point* center;
            SDL_Rect* clip;
};
}

#endif // SPRITE_H