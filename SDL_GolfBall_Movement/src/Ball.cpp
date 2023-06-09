#include "Ball.h"
#include "Vector2f.h"
#include "Sprite.h"

Ball::Ball() : Sprite(), velocity(0.0f, 0.0f), moving(false) {};

Ball::Ball(sdl::Texture* texture) : Sprite(texture), velocity(0.0f, 0.0f), moving(false) {};

Ball::Ball(sdl::Texture* texture, math::Vector2f position, math::Vector2f velocity)
: Sprite(texture, position), velocity(velocity), moving(false) {};

Ball::~Ball(){ }

void Ball::update(float dt){

    if(moving){
        velocity1D = velocity.magnitude();
        velocity.x *= pow(friction, dt) * ((velocity1D < 10.0f) ? 0.99f : 1.0f);
        velocity.y *= pow(friction, dt) * ((velocity1D < 10.0f) ? 0.99f : 1.0f);
        velocity1D = velocity.magnitude();

        setPosition(getPosition() + velocity * dt);

        if(velocity.x < 0.5f && velocity.x > -0.5f){
            velocity.x = 0.0f;
        }

        if(velocity.y < 0.5f && velocity.y > -0.5f){
            velocity.y = 0.0f;
        }

        if(velocity.x == 0.0f && velocity.y == 0.0f){
            moving = false;
        }
    }
}

void Ball::shrink(float shrink_factor, float dt){
    setScale(getScale() * shrink_factor * dt);
    setPosition(getPosition() + (getScale() * shrink_factor * dt) / 2.0f);
}

void Ball::setVelocity(math::Vector2f velocity){
    this->velocity = velocity;
}

void Ball::setVelocity(float x, float y){
    this->velocity.x = x;
    this->velocity.y = y;
}

void Ball::setMoving(bool moving){
    this->moving = moving;
}

bool Ball::isMoving() const {
    return moving;
}

math::Vector2f Ball::getVelocity() const {
    return velocity;
}