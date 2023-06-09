#ifndef BALL_H
#define BALL_H

#include "Sprite.h"

class Ball : public sdl::Sprite
{
    public:
        Ball();

        Ball(sdl::Texture* texture);

        Ball(sdl::Texture* texture, math::Vector2f position, math::Vector2f velocity);

        ~Ball();

        void update(float dt);

        void shrink(float shrink_factor, float dt);

        void setVelocity(math::Vector2f velocity);

        void setVelocity(float x, float y);

        void setMoving(bool moving);

        bool isMoving() const;

        math::Vector2f getVelocity() const;

    private:
        math::Vector2f velocity;
        float velocity1D = 0.0f;
        bool moving = false;
        const float friction = 0.6f;
};

#endif // BALL_H