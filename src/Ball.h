#ifndef BALL_H
#define BALL_H

#include "Sprite.h"
#include "Tile.h"

class Ball : public sdl::Sprite
{
    public:
        Ball();

        Ball(sdl::Texture* texture);

        Ball(sdl::Texture* texture, math::Vector2f position, math::Vector2f velocity);

        ~Ball();

        void update(float dt);

        void shrink(float shrink_factor);

        void setVelocity(math::Vector2f velocity);

        void setVelocity(float x, float y);

        void setVelocity1D(float velocity);

        float getVelocity1D();

        void setMoving(bool moving);

        bool isMoving() const;

        SDL_FRect getRect();

        math::Vector2f& getVelocity();

    private:
        math::Vector2f velocity;
        float velocity1D = 0.0f;
        bool moving = false;
        const float friction = 0.6f;
};

#endif // BALL_H